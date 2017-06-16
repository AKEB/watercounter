void WIFIinit() {
	// Попытка подключения к точке доступа
	WiFi.mode(WIFI_STA);
	digitalWrite(INNER_LED_PIN, LOW);
	
	byte tries = 15;
	WiFi.begin(_ssid.c_str(), _password.c_str());
	// Делаем проверку подключения до тех пор пока счетчик tries
	// не станет равен нулю или не получим подключение
	while (--tries && WiFi.status() != WL_CONNECTED) {
		Serial.print(".");
		digitalWrite(INNER_LED_PIN, HIGH);
		delay(500);
		digitalWrite(INNER_LED_PIN, LOW);
		delay(500);
	}
	
	if (WiFi.status() != WL_CONNECTED) {
		digitalWrite(INNER_LED_PIN, LOW);
		wifi_mode_time = 200;
		// Если не удалось подключиться запускаем в режиме AP
		Serial.println("");
		Serial.println("WiFi up AP");
		StartAPMode();
	} else {
		digitalWrite(INNER_LED_PIN, HIGH);
		wifi_mode_time = 2000;
		// Иначе удалось подключиться отправляем сообщение
		// о подключении и выводим адрес IP
		Serial.println("");
		Serial.println("WiFi connected");
		Serial.println("IP address: ");
		Serial.println(WiFi.localIP());
	}
}

bool StartAPMode() {
	IPAddress apIP(192, 168, 0, 1);
	// Отключаем WIFI
	WiFi.disconnect();
	// Меняем режим на режим точки доступа
	WiFi.mode(WIFI_AP);
	// Задаем настройки сети
	WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
	// Включаем WIFI в режиме точки доступа с именем и паролем
	// хронящихся в переменных _ssidAP _passwordAP
	WiFi.softAP(_ssidAP.c_str(), _passwordAP.c_str());
	return true;
}
