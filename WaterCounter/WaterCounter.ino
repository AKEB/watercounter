/*
   Шаг №8
   Обнавление прошивки из web интерфейса
   Получение времени из сети

*/
#include <ESP8266WiFi.h> 
#include <ESP8266WebServer.h>
#include <ESP8266SSDP.h> 
#include <FS.h>
#include <ArduinoJson.h>

#include <ESP8266HTTPUpdateServer.h>



#define RED_LED_PIN 15
#define BLUE_LED_PIN 13
#define INNER_LED_PIN 2

#define COLD_WATER_PIN 14
#define HOT_WATER_PIN 4
#define ALARM_WATER_PIN 12



// Объект для обнавления с web страницы 
ESP8266HTTPUpdateServer httpUpdater;

// Web интерфейс для устройства
ESP8266WebServer HTTP;

// Для файловой системы
File fsUploadFile;

// Определяем переменные wifi
String _ssid     = "{SSID}"; // Для хранения SSID
String _password = "{pasword}"; // Для хранения пароля сети
String _ssidAP = "WaterCounter";   // SSID AP точки доступа
String _passwordAP = ""; // пароль точки доступа
String SSDP_Name = "WaterCounter"; // Имя SSDP
int timezone = 3;               // часовой пояс GTM
int ColdWaterCount = 0;
int HotWaterCount = 0;
int Alert = 0;
int SaveCount = 0;

String jsonConfig = "{}";
int port = 80;

int wifi_mode_time = 2000;
unsigned long wifi_mode_previous_millis = 0;
int inner_led_state = LOW;


int save_time_interval = 60*60*1000;
unsigned long save_previous_millis = 0;

int HighMillis=0;
int Rollover=0;

void setup() {
	
	pinMode(RED_LED_PIN, OUTPUT);
	pinMode(BLUE_LED_PIN, OUTPUT);
	pinMode(INNER_LED_PIN, OUTPUT);

	pinMode(COLD_WATER_PIN, INPUT_PULLUP);
	pinMode(HOT_WATER_PIN, INPUT_PULLUP);
	pinMode(ALARM_WATER_PIN, INPUT_PULLUP);

	
	HTTP = ESP8266WebServer (port);
	Serial.begin(115200);
	Serial.println("");
	//Запускаем файловую систему
	Serial.println("Start File System");
	FS_init();
	Serial.println("Load Config File");
	loadConfig();
	Serial.println("Start WiFi");
	//Запускаем WIFI
	WIFIinit();
	Serial.println("Start 8-Time");
	// Получаем время из сети
	Time_init();
	//Настраиваем и запускаем SSDP интерфейс
	Serial.println("Start 3-SSDP");
	SSDP_init();
	//Настраиваем и запускаем HTTP интерфейс
	Serial.println("Start 2-WebServer");
	HTTP_init();

}

void loop() {
	unsigned long currentMillis = millis();
	
	if(currentMillis>=3000000000){ 
		HighMillis=1;
	}
	//** Making note of actual rollover **//
	if(currentMillis<=100000 && HighMillis==1){
		Rollover++;
		HighMillis=0;
	}
	
	HTTP.handleClient();
	delay(1);
	
	// Мигаем встроенной лампочкой
	if (currentMillis < wifi_mode_previous_millis) wifi_mode_previous_millis = 0;
	if(currentMillis - wifi_mode_previous_millis >= wifi_mode_time) {
		wifi_mode_previous_millis = currentMillis;   
		if (inner_led_state == LOW)
 			inner_led_state = HIGH;  // Note that this switches the LED *off*
		else
			inner_led_state = LOW;   // Note that this switches the LED *on*
		digitalWrite(INNER_LED_PIN, inner_led_state);
	}

	// Сохраняем файл настроек раз в час, если есть изменения
	if (currentMillis < save_previous_millis) save_previous_millis = 0;
	if (currentMillis - save_previous_millis >= save_time_interval) {
		Serial.println("Check Save Config");
		save_previous_millis = currentMillis;
		saveConfig();
	}
}



