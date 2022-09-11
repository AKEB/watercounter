void SSDP_init(void) {
	// SSDP дескриптор
	HTTP.on("/description.xml", HTTP_GET, []() {
		SSDP.schema(HTTP.client());
	});
	//Если версия  2.0.0 закаментируйте следующую строчку
	SSDP.setDeviceType("upnp:rootdevice");
	SSDP.setSchemaURL("description.xml");
	SSDP.setHTTPPort(80);
	SSDP.setName(SSDP_Name);
	SSDP.setSerialNumber("000000000002");
	SSDP.setURL("/");
	SSDP.setModelName("WaterCounter");
	SSDP.setModelNumber("000000000002");
	SSDP.setModelURL("https://github.com/AKEB/watercounter");
	SSDP.setManufacturer("Vadim Babajanyan");
	SSDP.setManufacturerURL("http://akeb.ru");
	SSDP.begin();
}
