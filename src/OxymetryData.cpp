#include <OxymetryData.h>


OxymetryData::OxymetryData(SoftwareSerial* swSer)
{
    swSerial = swSer;
}

void OxymetryData::begin(int32_t BAUD_RATE){
    
    swSerial->begin(BAUD_RATE, SWSERIAL_8N1);
}

void OxymetryData::read(){
    uint8_t bytes[5];
	unsigned long starttime;
	starttime = millis();

	while ((swSerial->available() < 5) && ((millis() - starttime) < 300)){}

	if (swSerial->available() < 5)
	{
		// the data didn't come in - handle that problem here
		// Serial.println("ERROR - Didn't get 5 bytes of data!");
	}
	else
	{
		for (int i = 0; i < 5; i++)
		{
			bytes[i] = swSerial->read();
            int bit[8];
			getBitsFromByte(bytes[i], bit);
			// pulseOxymetry[i](bit);
		}
	}
}

void OxymetryData::getBitsFromByte(uint8_t _byte, int bit[8])
{

	bit[7] = (_byte & 128) >> 7;
	bit[6] = (_byte & 64) >> 6;
	bit[5] = (_byte & 32) >> 5;
	bit[4] = (_byte & 16) >> 4;
	bit[3] = (_byte & 8) >> 3;
	bit[2] = (_byte & 4) >> 2;
	bit[1] = (_byte & 2) >> 1;
	bit[0] = (_byte & 1) >> 0;
}

OxymetryData::~OxymetryData()
{
}