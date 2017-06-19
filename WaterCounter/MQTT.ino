void MQTT_init() {
	HTTP.on("/mqtt", handle_Set_MQTT); // Установка настроек MQTT Сервера /mqtt?mqtt_host=192.168.1.200&mqtt_port=1883&mqtt_user=&mqtt_password=
	
	MQTT_connect();
}

void MQTT_loop() {
	if (WiFi.status() != WL_CONNECTED) return;
	
	if (!clientForMQTT.connected()) {
		if (currentMillis < mqtt_reconnect_previous_millis) mqtt_reconnect_previous_millis = 0;
		if(currentMillis - mqtt_reconnect_previous_millis >= mqtt_reconnect_interval) {
			mqtt_reconnect_previous_millis = currentMillis;
			if (MQTT_reconnect()) {
				//mqtt_reconnect_previous_millis = 0;
			}
		}
	} else {
		clientForMQTT.loop();
	}
	
}

boolean MQTT_reconnect() {
	error_log("MQTT reconnect");
	MQTT_connect();
	return clientForMQTT.connected();
}

void MQTT_connect() {
	error_log("MQTT connect");
	if (!_mqtt_host || _mqtt_port < 1) return;
	
	if (WiFi.status() != WL_CONNECTED) return;
	
	error_log("MQTT set Server IP and PORT");
	clientForMQTT.setServer(_mqtt_host.c_str(), _mqtt_port);
	clientForMQTT.setCallback(callbackForMQTT);
	
	error_log("MQTT connecting...");
	
	if (!clientForMQTT.connected()) {
		wifiClientForMQTT.stop();
		if (_mqtt_user) {
			if (!clientForMQTT.connect(_ssidAP.c_str(), _mqtt_user.c_str(), _mqtt_password.c_str())) {
				error_log("MQTT can't connect");
				return;
			}
		} else {
			if (!clientForMQTT.connect(_ssidAP.c_str())) {
				error_log("MQTT can't connect");
				return;
			}
		}
	}
	
	error_log("MQTT connected");

	// Подписываемся на топики
	String topic = "home/"+ _ssidAP + "/#";
	error_log("MQTT subscribe to topic "+topic);
	clientForMQTT.subscribe(topic.c_str());
}

void MQTT_publish(String topic, String message) {
	if (WiFi.status() != WL_CONNECTED) return;
	error_log("MQTT publish " + topic + " = " + message);
	if (!clientForMQTT.connected()) {
		MQTT_reconnect();
	}
	
	clientForMQTT.publish(topic.c_str(), message.c_str());
}

//  Функция вызывается, когда есть изменения по топику на который мы подписаны!
void callbackForMQTT(char* topic, byte* payload, unsigned int length) {
	error_log("MQTT callbackForMQTT");
	
	String topic_String = (String) topic;
	String payload_String = (String)((char *)payload);

	payload_String = payload_String.substring(0,length);
	
	error_log("incoming: "+topic_String+" - "+payload_String);
	boolean saveConfigNeed = false;
	String t = "home/"+ _ssidAP + "/";
	
	topic_String.remove(0, t.length());

	if (topic_String.equals("cold")) {
		if (ColdWaterCount != payload_String.toInt()) saveConfigNeed = true;
		ColdWaterCount = payload_String.toInt();
	} else if (topic_String.equals("hot")) {
		if (HotWaterCount != payload_String.toInt()) saveConfigNeed = true;
		HotWaterCount = payload_String.toInt();
	} else if (topic_String.equals("alert")) {
		if (Alert != payload_String.toInt()) saveConfigNeed = true;
		Alert = payload_String.toInt();
	}

	if (saveConfigNeed) saveConfig();
}

void handle_Set_MQTT() {
	if(!HTTP.authenticate(_http_user.c_str(), _http_password.c_str())) return HTTP.requestAuthentication();
	_mqtt_host = HTTP.arg("mqtt_host");
	_mqtt_port = HTTP.arg("mqtt_port").toInt();
	_mqtt_user = HTTP.arg("mqtt_user");
	_mqtt_password = HTTP.arg("mqtt_password");
	saveConfig();                         // Функция сохранения данных во Flash
	HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
	
	MQTT_connect();
}

