#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <Filters.h>

#define MAX_MILLIS_TO_WAIT 1000 //or whatever

FilterOnePole lpf( LOWPASS, 0.05 );

SoftwareSerial swSer;

void setup()
{
	swSer.begin(19200, SWSERIAL_8O1, D5, D6);
	Serial.begin(115200);
}

int strength = 1;

int statebyte = -1;

bool data_found = false;
int pulse = 0;

void loop()
{
	char c[5];
	unsigned long starttime;
	starttime = millis();

	while ((swSer.available() < 5) && ((millis() - starttime) < MAX_MILLIS_TO_WAIT))
	// while ((swSer.available()<5))

	{
	}

	if (swSer.available() < 5)
	{
		// the data didn't come in - handle that problem here
		// Serial.println("ERROR - Didn't get 5 bytes of data!");
		Serial.flush();
		data_found = false;
	}
	else
	{
		data_found = true;

		for (int i = 0; i < 5; i++)
		{
			c[i] = swSer.read();
		}
	}

	if (data_found)
	{
		for (int i = 0; i < 5; i++)
		{

			
			int bit[8];

			bit[7] = (c[i] & 128) >> 7;
			bit[6] = (c[i] & 64) >> 6;
			bit[5] = (c[i] & 32) >> 5;
			bit[4] = (c[i] & 16) >> 4;
			bit[3] = (c[i] & 8) >> 3;
			bit[2] = (c[i] & 4) >> 2;
			bit[1] = (c[i] & 2) >> 1;
			bit[0] = (c[i] & 1) >> 0;

			// for (int j = 0; j < 8; j++)
			// {
			// 	Serial.print(bit[j]);
			// 	Serial.print(" ");
			// }

			// Serial.print(c[i], DEC);
			// Serial.println("");

			if (bit[7] == 1 && statebyte <= 0)
			{
				// Byte 1
				statebyte = 1;

				if ((bit[4] != 0) || (bit[5] != 0))
				{
					// Serial.println("Searching too long dropping Sp02");
				}
				strength = (bit[3] * 8) + (bit[2] * 4) + (bit[1] * 2) + bit[0];
				// Serial.print("Signal Strength:");
				// Serial.println(strength);
			}
			else
			{
				if (statebyte > 0)
				{
					if (bit[7] != 0)
					{
						// Serial.println("Protocol error");
						strength = 0;
						// break
					}
					statebyte = statebyte + 1;

					if (statebyte == 2){
						pulse = (bit[6] * 64) + (bit[5] * 32) + (bit[4] * 16) + (bit[3] * 8) + (bit[2] * 4) + (bit[1] * 2) + bit[0];
					}
					if (statebyte == 3){
						pulse = pulse + (bit[7] * 128);
						// Serial.print("Pulse: ");
						Serial.print(pulse);
						Serial.print("\t");
						Serial.println(lpf.input(pulse));					
					}

					if (statebyte == 5)
						statebyte = 0;
				}
			}
		}
	}
}