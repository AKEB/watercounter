void MQTT_init() {
	
	HTTP.on("/mqtt", handle_Set_MQTT); // Установка настроек MQTT Сервера /mqtt?mqtt_host=192.168.1.200&mqtt_port=1883&mqtt_user=&mqtt_password=
	
}

void MQTT_loop() {
	
}

void handle_Set_MQTT() {
	_mqtt_host = HTTP.arg("mqtt_host");
	_mqtt_port = HTTP.arg("mqtt_port").toInt();
	_mqtt_user = HTTP.arg("mqtt_user");
	_mqtt_password = HTTP.arg("mqtt_password");
	saveConfig();                         // Функция сохранения данных во Flash
	HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
}

