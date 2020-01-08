#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
// #include <Filters.h>
#include <OxiParser.h>
// #include <socketEvents.h>

// FilterOnePole lpf(LOWPASS, 0.05);

SoftwareSerial swSer(D5, D6, false);

// WebSocketsClient webSocket;

OxiParser oxiParser(&swSer);

#define USE_SERIAL Serial

void readraw(byte data){
        // int buff[8];
        // oxiParser.getBitsFromByte(data, buff);
        // for (int i=7; i>=0; i--){
        // Serial.print(buff[i]);
        // }
        // Serial.print('\n');

    }
void readspo2(byte data){
    // Serial.println(data);
}

void readsig(byte data){
    // Serial.println(data);
}
void readpulse(byte data){
    // Serial.println(data);
}
void readpleth(byte data){
    Serial.println(data);
}

void setup()
{
    oxiParser.begin();
    USE_SERIAL.begin(115200);

	//Serial.setDebugOutput(true);
	USE_SERIAL.setDebugOutput(true);

	USE_SERIAL.println();
	USE_SERIAL.println();
	USE_SERIAL.println();

    for(uint8_t t = 4; t > 0; t--) {
		USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
		USE_SERIAL.flush();
		delay(1000);
	}

    oxiParser.readRaw(readraw);
    
    oxiParser.readPleth(readpleth);
    oxiParser.readSignalStrength(readsig);
    oxiParser.readSpo2(readspo2);
    oxiParser.readPulseRate(readpulse);

	// WiFi.begin("BIOFORGELABS1", "99chocolatechipcookies");
	// // WiFi.disconnect();
	// while(WiFi.status() != WL_CONNECTED) {
	// 	delay(100);
	// }

	// // server address, port and URL
	// webSocket.begin("localhost", 8080, "/");

	// // event handler
	// webSocket.onEvent(webSocketEvent);

	// // use HTTP Basic Authorization this is optional remove if not needed
	// // webSocket.setAuthorization("user", "Password");

	// // try ever 5000 again if connection has failed
	// webSocket.setReconnectInterval(5000);
}


void loop()
{
    
    
    
oxiParser.loop();
    // static unsigned long prev_millis = millis();
    // static int a, b, c, d;
    // int spo2 = oxiParser.getSpo2();
    // int pr = oxiParser.getPulseRate();
    // int ss = oxiParser.getSignalStrength();
    // int pleth = oxiParser.getPleth();
    // static int interval=0;
    // if(d!=pleth){
    // if (interval > 1)
    // {
        // Serial.print(pleth);
        // Serial.print(" ");
        // Serial.print(millis());
        // Serial.print("\n");
        // delay(50);
        // interval = 0;
    // }
    // else
    // {
    //     interval++;
    // }

    // d=pleth;
    // }

    // if(a!=spo2 || b!=pr || c!=ss){
    //     Serial.print(oxiParser.getSpo2());
    //     Serial.print('\t');
    //     Serial.print(oxiParser.getPulseRate());
    //     Serial.print('\t');
    //     Serial.print(oxiParser.getSignalStrength());
    //     Serial.println('\t');
    //     a=spo2;
    //     b=pr;
    //     c=ss;
    // }
}