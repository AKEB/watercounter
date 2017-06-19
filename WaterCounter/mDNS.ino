#include <ESP8266mDNS.h>

void mDNS_init() {
	MDNS.begin(SSDP_Name.c_str());
	
	MDNS.addService("http", "tcp", 80);
	//MDNS.addService("telnet", "telnet", 23);
	
}

