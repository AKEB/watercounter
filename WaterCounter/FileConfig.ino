
// Загрузка данных сохраненных в файл  config.json
bool loadConfig() {
	// Открываем файл для чтения
	File configFile = SPIFFS.open("/config.json", "r");
	if (!configFile) {
		// если файл не найден  
		Serial.println("Failed to open config file");
		// Создаем файл запиав в него аные по умолчанию
		saveConfig();
		configFile.close();
		return false;
	}
	// Проверяем размер файла, будем использовать файл размером меньше 1024 байта
	size_t size = configFile.size();
	if (size > 1024) {
		Serial.println("Config file size is too large");
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
	
	/*
	File configFile = SPIFFS.open("/config.json", "r");
	if (configFile) {
		String oldString = configFile.readString();
		configFile.close();

		if (oldString.equals(jsonConfig)) return true;
	}
	*/
	Serial.println("Save File");
	// Открываем файл для записи
	File configFile = SPIFFS.open("/config.json", "w");
	if (!configFile) {
		//Serial.println("Failed to open config file for writing");
		configFile.close();
		return false;
	}
	// Записываем строку json в файл 
	json.printTo(configFile);
	configFile.close();
	
	return true;
}

