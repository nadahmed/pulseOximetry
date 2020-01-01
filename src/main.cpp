#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <Filters.h>
#include <OxiParser.h>


FilterOnePole lpf(LOWPASS, 0.05);

SoftwareSerial swSer(D5, D6, false);

OxiParser oxiParser(&swSer);

void setup()
{
	oxiParser.begin();
	Serial.begin(115200);
	Serial.flush();
	Serial.print('\n');
}



void loop()
{
	oxiParser.read();
    Serial.print(oxiParser.getSpo2());
    Serial.print('\t');
    Serial.print(oxiParser.getPulseRate());
    Serial.print('\t');
    Serial.print(oxiParser.getSignalStrength());
    Serial.print('\t');
}