
void Telnet_init() {
	TelnetServer.begin();
	TelnetServer.setNoDelay(true);

}

void Telnet_error_log(String str,boolean ln) {
	for (byte i = 0; i < MAX_SRV_CLIENTS; i++) {
		if (TelnetClients[i]) {
			TelnetClients[i].print(str);
			if (ln) TelnetClients[i].println("");
		}
	}
}

void Telnet_loop() {
	
	// проверяем наличие нового клиента по Telnet
	WiFiClient client1 = TelnetServer.available();
	if (client1) {
		boolean newClient = true;
		for (byte i = 0; i < 10; i++) {
			//check whether this client refers to the same socket as one of the existing instances:
			if (TelnetClients[i] == client1) {
				newClient = false;
				break;
			}
		}
		if (newClient) {
			//check which of the existing clients can be overridden:
			for (byte i = 0; i < 10; i++) {
				if (!TelnetClients[i] && TelnetClients[i] != client1) {
					TelnetClients[i] = client1;
					// clear out the input buffer:
					client1.flush();
					Serial.println("We have a new telnet client");
					client1.print("Hello, client number: ");
					client1.print(i);
					client1.println();
					break;
				}
			}
		}
	}
	// стираем отключенных клиентов
	for (byte i = 0; i < 10; i++) {
		if (!(TelnetClients[i].connected())) {
			TelnetClients[i].stop();
		}
	}
	
}

