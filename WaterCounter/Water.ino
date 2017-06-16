
void Water_init() {
	pinMode(RED_LED_PIN, OUTPUT);
	pinMode(BLUE_LED_PIN, OUTPUT);
	
	pinMode(COLD_WATER_PIN, INPUT_PULLUP);
	pinMode(HOT_WATER_PIN, INPUT_PULLUP);
	pinMode(ALARM_WATER_PIN, INPUT_PULLUP);
	
	
	HTTP.on("/watercold", handle_Set_WaterCold);
	HTTP.on("/waterhot", handle_Set_WaterHot);
}

void Water_loop() {
	
}

void handle_Set_WaterCold() {
	ColdWaterCount = HTTP.arg("cold").toInt();
	saveConfig();                         // Функция сохранения данных во Flash
	HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
}

void handle_Set_WaterHot() {
	HotWaterCount = HTTP.arg("hot").toInt();
	saveConfig();                         // Функция сохранения данных во Flash
	HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
}
