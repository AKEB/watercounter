#include <time.h>               //Содержится в пакете

void Time_init() {
	HTTP.on("/Time", handle_Time);     // Синхронизировать время устройства по запросу вида /Time
	HTTP.on("/TimeZone", handle_time_zone);    // Установка времянной зоны по запросу вида /TimeZone?timezone=3
	timeSynch(timezone);
	currentMillis = millis();
}

void Time_loop() {
	currentMillis = millis();
	
	if(currentMillis>=3000000000){ 
		HighMillis=1;
	}
	//** Making note of actual rollover **//
	if(currentMillis<=100000 && HighMillis==1){
		Rollover++;
		HighMillis=0;
	}
}

void timeSynch(int zone){
	if (WiFi.status() == WL_CONNECTED) {
		// Настройка соединения с NTP сервером
		configTime(zone * 3600, 0, "pool.ntp.org", "ru.pool.ntp.org");
		int i = 0;
		error_log("\nWaiting for time");
		while (!time(nullptr) && i < 10) {
			error_log(".", false);
			i++;
			delay(1000);
		}
		error_log("");
		error_log("ITime Ready!");
		error_log(GetTime());
		error_log(GetDate());
	}
}

// Установка параметров времянной зоны по запросу вида http://192.168.0.101/TimeZone?timezone=3
void handle_time_zone() {
	if(!HTTP.authenticate(_http_user.c_str(), _http_password.c_str())) return HTTP.requestAuthentication();
	timezone = HTTP.arg("timezone").toInt(); // Получаем значение timezone из запроса конвертируем в int сохраняем в глобальной переменной
	saveConfig();
	HTTP.send(200, "text/plain", "OK");
}

void handle_Time(){
	if(!HTTP.authenticate(_http_user.c_str(), _http_password.c_str())) return HTTP.requestAuthentication();
	timeSynch(timezone);
	HTTP.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
}

// Получение текущего времени
String GetTime() {
	time_t now = time(nullptr); // получаем время с помощью библиотеки time.h
	String Time = ""; // Строка для результатов времени
	Time += ctime(&now); // Преобразуем время в строку формата Thu Jan 19 00:55:35 2017
	int i = Time.indexOf(":"); //Ишем позицию первого символа :
	Time = Time.substring(i - 2, i + 6); // Выделяем из строки 2 символа перед символом : и 6 символов после
	return Time; // Возврашаем полученное время
}

// Получение даты
String GetDate() {
	time_t now = time(nullptr); // получаем время с помощью библиотеки time.h
	String Data = ""; // Строка для результатов времени
	Data += ctime(&now); // Преобразуем время в строку формата Thu Jan 19 00:55:35 2017
	int i = Data.lastIndexOf(" "); //Ишем позицию последнего символа пробел
	String Time = Data.substring(i - 8, i+1); // Выделяем время и пробел
	Data.replace(Time, ""); // Удаляем из строки 8 символов времени и пробел
	return Data; // Возврашаем полученную дату
}

// Function made to millis() be an optional parameter
char *uptime() {
	return (char *)uptime(millis()); // call original uptime function with unsigned long millis() value
}

char *uptime(unsigned long milli) {
	static char _return[32];
	unsigned long secs=milli/1000, mins=secs/60;
	unsigned int hours=mins/60, days=hours/24;
	milli-=secs*1000;
	secs-=mins*60;
	mins-=hours*60;
	hours-=days*24;
	days = days+ (Rollover*50);
	sprintf(_return,"Uptime %d days %02d:%02d:%02d.%03d", (byte)days, (byte)hours, (byte)mins, (byte)secs, (int)milli);
	return _return;
}
