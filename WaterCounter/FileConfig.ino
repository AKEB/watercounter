
void FileConfig_loop() {
	// Сохраняем файл настроек раз в час, если есть изменения
	if (currentMillis < save_previous_millis) save_previous_millis = 0;
	if (currentMillis - save_previous_millis >= save_time_interval) {
		error_log("Check Save Config");
		save_previous_millis = currentMillis;
		saveConfig();
	}
}

// Загрузка данных сохраненных в файл  config.json
bool loadConfig() {
	// Открываем файл для чтения
	File configFile = SPIFFS.open("/config.json", "r");
	if (!configFile) {
		// если файл не найден  
		error_log("Failed to open config file");
		// Создаем файл запиав в него аные по умолчанию
		saveConfig();
		configFile.close();
		return false;
	}
	// Проверяем размер файла, будем использовать файл размером меньше 1024 байта
	size_t size = configFile.size();
	if (size > 1024) {
		error_log("Config file size is too large");
		configFile.close();
		return false;
	}
	
	// загружаем файл конфигурации в глобальную переменную
	jsonConfig = configFile.readString();
	configFile.close();
	// Резервируем памяь для json обекта буфер может рости по мере необходимти предпочтительно для ESP8266 
	DynamicJsonBuffer jsonBuffer;
	// вызовите парсер JSON через экземпляр jsonBuffer
	// строку возьмем из глобальной переменной String jsonConfig
	JsonObject& root = jsonBuffer.parseObject(jsonConfig);
	// Теперь можно получить значения из root  
	_ssidAP = root["ssidAPName"].as<String>(); // Так получаем строку
	_passwordAP = root["ssidAPPassword"].as<String>();
	timezone = root["timezone"];               // Так получаем числа
	SSDP_Name = root["SSDPName"].as<String>();
	_ssid = root["ssidName"].as<String>();
	_password = root["ssidPassword"].as<String>();
	ColdWaterCount = root["ColdWaterCount"];
	HotWaterCount = root["HotWaterCount"];
	Alert = root["Alert"];
	SaveCount = root["SaveCount"];
	
	_mqtt_host = root["mqtt_host"].as<String>();
	_mqtt_port = root["mqtt_port"];
	_mqtt_user = root["mqtt_user"].as<String>();
	_mqtt_password = root["mqtt_password"].as<String>();

	_http_user = root["http_user"].as<String>();
	_http_password = root["http_password"].as<String>();
	return true;
}

// Запись данных в файл config.json
bool saveConfig() {
	// Резервируем память для json обекта буфер может рости по мере необходимти предпочтительно для ESP8266 
	DynamicJsonBuffer jsonBuffer;
	// вызовите парсер JSON через экземпляр jsonBuffer
	JsonObject& json = jsonBuffer.parseObject(jsonConfig);
	// Заполняем поля json 
	json["SSDPName"] = SSDP_Name;
	json["ssidAPName"] = _ssidAP;
	json["ssidAPPassword"] = _passwordAP;
	json["ssidName"] = _ssid;
	json["ssidPassword"] = _password;
	json["timezone"] = timezone;
	json["ColdWaterCount"] = ColdWaterCount;
	json["HotWaterCount"] = HotWaterCount;
	json["Alert"] = Alert;
	json["SaveCount"] = SaveCount;
	
	json["mqtt_host"] = _mqtt_host;
	json["mqtt_port"] = _mqtt_port;
	json["mqtt_user"] = _mqtt_user;
	json["mqtt_password"] = _mqtt_password;

	json["http_user"] = _http_user;
	json["http_password"] = _http_password;
	
	
	// Помещаем созданный json в глобальную переменную json.printTo(jsonConfig);
	String NewString = "";
	json.printTo(NewString);
	
	if (jsonConfig.equals(NewString)) {
		return true;
	}

	SaveCount = SaveCount + 1;
	json["SaveCount"] = SaveCount;
	jsonConfig = "";
	json.printTo(jsonConfig);
	
	error_log("Save File");
	// Открываем файл для записи
	File configFile = SPIFFS.open("/config.json", "w");
	if (!configFile) {
		configFile.close();
		return false;
	}
	// Записываем строку json в файл 
	json.printTo(configFile);
	configFile.close();
	
	return true;
}


