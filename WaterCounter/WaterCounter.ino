#include <ESP8266WiFi.h> 
#include <ESP8266WebServer.h>
#include <ESP8266SSDP.h> 
#include <FS.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
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
String _ssid     = "AKEB"; // Для хранения SSID
String _password = "{password}"; // Для хранения пароля сети
String _ssidAP = "WaterCounter";   // SSID AP точки доступа
String _passwordAP = ""; // пароль точки доступа
IPAddress apIP(192, 168, 0, 1);

String _http_user = "admin";
String _http_password = "0000";

String _mqtt_host     = "192.168.1.200";
int _mqtt_port     = 1883;
String _mqtt_user     = "";
String _mqtt_password     = "";

String SSDP_Name = "WaterCounter"; // Имя SSDP
int timezone = 3;               // часовой пояс GTM
int ColdWaterCount = 0;
int HotWaterCount = 0;
int Alert = 0;
int SaveCount = 0;

int ColdWaterState = 0;
int HotWaterState = 0;
int AlertState = 0;

String jsonConfig = "{}";
int port = 80;

int HighMillis=0;
int Rollover=0;

unsigned long currentMillis;
int save_time_interval = 60*60*1000;
unsigned long save_previous_millis = 0;
int wifi_mode_time = 2000;
int wifi_mode = 0;
unsigned long wifi_mode_previous_millis = 0;
int inner_led_state = LOW;
unsigned long mqtt_reconnect_previous_millis = 0;
int mqtt_reconnect_interval = 5000;
unsigned long water_send_previous_millis = 0;
int water_send_interval = 10000;
unsigned long water_previous_millis = 0;
int water_interval = 500;
unsigned long alert_led_previous_millis = 0;
int alert_led_interval = 100;
int alert_led_state = LOW;

boolean water_changes_for_send = true;

WiFiClient wifiClientForMQTT;
PubSubClient clientForMQTT(wifiClientForMQTT);

#define MAX_SRV_CLIENTS 10
WiFiServer TelnetServer(23);
WiFiClient TelnetClients[MAX_SRV_CLIENTS];

void error_log(String str) {
	error_log(str, true);
}

void error_log(String str, boolean ln) {
	Serial.print(str);
	
	if (ln) Serial.println("");
	
	Telnet_error_log(str, ln);
}

void setup() {
	HTTP = ESP8266WebServer (port);
	
	Telnet_init();
	
	Serial.begin(115200);
	error_log("");
	
	//Запускаем файловую систему
	error_log("Start File System");
	FS_init();
	
	error_log("Load Config File");
	loadConfig();
	
	error_log("Start WiFi");
	//Запускаем WIFI
	WIFI_init();
	
	error_log("Start Time module");
	// Получаем время из сети
	Time_init();

	error_log("Start MQTT module");
	MQTT_init();
	
	//Настраиваем и запускаем SSDP интерфейс
	error_log("Start SSDP");
	SSDP_init();
	
	//Настраиваем и запускаем HTTP интерфейс
	error_log("Start WebServer");
	HTTP_init();
	
	//Настраиваем и запускаем HTTP интерфейс
	error_log("Start mDNS");
	mDNS_init();
	
	error_log("Start Water module");
	Water_init();
}

void loop() {
	HTTP.handleClient();
	
	Telnet_loop();
	
	delay(1);
	Time_loop();
	
	WIFI_loop();

	FileConfig_loop();

	MQTT_loop();

	Water_loop();
	
}



