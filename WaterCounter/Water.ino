
void Water_init() {
	pinMode(RED_LED_PIN, OUTPUT);
	pinMode(BLUE_LED_PIN, OUTPUT);
	
	pinMode(COLD_WATER_PIN, INPUT_PULLUP);
	pinMode(HOT_WATER_PIN, INPUT_PULLUP);
	pinMode(ALARM_WATER_PIN, INPUT_PULLUP);
	
	water_changes_for_send = true;
	
	HTTP.on("/watercold", handle_Set_WaterCold);
	HTTP.on("/waterhot", handle_Set_WaterHot);
	HTTP.on("/waterAlert", handle_Set_WaterAlert);
	
}

void Water_loop() {
	
	if (currentMillis < water_send_previous_millis) water_send_previous_millis = 0;
	if(currentMillis - water_send_previous_millis >= water_send_interval) {
		water_send_previous_millis = currentMillis;
		
		if (water_changes_for_send) {
			MQTT_publish("home/"+ _ssidAP + "/cold",(String) ColdWaterCount);
			MQTT_publish("home/"+ _ssidAP + "/hot",(String) HotWaterCount);
			MQTT_publish("home/"+ _ssidAP + "/alert",(String) Alert);
			water_changes_for_send = false;
		}
	}
	
	if (currentMillis < water_previous_millis) water_previous_millis = 0;
	if(currentMillis - water_previous_millis >= water_interval) {
		water_previous_millis = currentMillis;
		
		int state = 0;
		state = digitalRead(COLD_WATER_PIN);
		if (state != ColdWaterState) {
			ColdWaterState = state;
			if (ColdWaterState == LOW) {
				++ColdWaterCount;
				MQTT_publish("home/"+ _ssidAP + "/cold",(String) ColdWaterCount);
			}
		}
		if (Alert < 1) digitalWrite(BLUE_LED_PIN, !state);
		
		state = digitalRead(HOT_WATER_PIN);
		if (state != HotWaterState) {
			HotWaterState = state;
			if (HotWaterState == LOW) {
				++HotWaterCount;
				MQTT_publish("home/"+ _ssidAP + "/hot",(String) HotWaterCount);
			}
		}
		if (Alert < 1) digitalWrite(RED_LED_PIN, !state);
		
		state = digitalRead(ALARM_WATER_PIN);
		if (state != AlertState) {
			AlertState = state;
			if (AlertState == LOW) {
				++Alert;
				MQTT_publish("home/"+ _ssidAP + "/alert",(String) Alert);
			}
		}
	}

	if (currentMillis < alert_led_previous_millis) alert_led_previous_millis = 0;
	if(currentMillis - alert_led_previous_millis >= alert_led_interval) {
		if (Alert > 0) {
			alert_led_previous_millis = currentMillis;
			alert_led_state = !alert_led_state;
			digitalWrite(RED_LED_PIN, alert_led_state);
			digitalWrite(BLUE_LED_PIN, alert_led_state);
		}
	}
}

void handle_Set_WaterAlert() {
	if(!HTTP.authenticate(_http_user.c_str(), _http_password.c_str())) return HTTP.requestAuthentication();
	error_log("handle_Set_WaterAlert disable=" + HTTP.arg("disable"));
	if (HTTP.arg("disable").toInt() > 0) {
		Alert = 0;
	}
	water_changes_for_send = true;
	saveConfig();                         // Функция сохранения данных во Flash
	HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
}

void handle_Set_WaterCold() {
	if(!HTTP.authenticate(_http_user.c_str(), _http_password.c_str())) return HTTP.requestAuthentication();
	ColdWaterCount = HTTP.arg("cold").toInt();
	water_changes_for_send = true;
	saveConfig();                         // Функция сохранения данных во Flash
	HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
}

void handle_Set_WaterHot() {
	if(!HTTP.authenticate(_http_user.c_str(), _http_password.c_str())) return HTTP.requestAuthentication();
	HotWaterCount = HTTP.arg("hot").toInt();
	water_changes_for_send = true;
	saveConfig();                         // Функция сохранения данных во Flash
	HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
}
