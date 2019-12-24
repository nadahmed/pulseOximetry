#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <Filters.h>

#define MAX_MILLIS_TO_WAIT 1000 //or whatever

FilterOnePole lpf(LOWPASS, 0.05);

SoftwareSerial swSer;

void getBitsFromByte(byte b, int bit[8]);
void package1(int bit[8]);
void package2(int bit[8]);
void package3(int bit[8]);
void package4(int bit[8]);
void package5(int bit[8]);


void setup()
{
	swSer.begin(19200, SWSERIAL_8O1, D5, D6);
	Serial.begin(115200);
	Serial.flush();
	Serial.print('\n');
}

int strength = 1;

int statebyte = -1;

bool data_found = false;
int pulse = 0;

void loop()
{
	byte b[5];
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
			b[i] = swSer.read();
		}
	}

	if (data_found)
	{
		for (int i = 0; i < 5; i++)
		{
			Serial.print("Byte no. : ");
			Serial.println(i);
			// Serial.print(b[i], BIN);
			Serial.print('\n');
			int bit[8];
			getBitsFromByte(b[i], bit);
			// for (int i = 7; i >= 0; i--)
			// {
			// 	Serial.print(bit[i]);
			// }
			// Serial.print('\n');

			switch (i)
			{
			case 0:
				package1(bit);
				break;
			
			case 1:
				package2(bit);
				break;

			case 2:
				package3(bit);
				break;

			case 3:
				package4(bit);
				break;

			case 4:
				package5(bit);
				break;
			
			default:
				break;
			}

		}
	}
}

// 			// for (int j = 0; j < 8; j++)
// 			// {
// 			// 	Serial.print(bit[j]);
// 			// 	Serial.print(" ");
// 			// }

// 			// Serial.print(c[i], DEC);
// 			// Serial.println("");

// 			if (bit[7] == 1 && statebyte <= 0)
// 			{
// 				// Byte 1
// 				statebyte = 1;

// 				if ((bit[4] != 0) || (bit[5] != 0))
// 				{
// 					// Serial.println("Searching too long dropping Sp02");
// 				}
// 				strength = (bit[3] * 8) + (bit[2] * 4) + (bit[1] * 2) + bit[0];
// 				// Serial.print("Signal Strength:");
// 				// Serial.println(strength);
// 			}
// 			else
// 			{
// 				if (statebyte > 0)
// 				{
// 					if (bit[7] != 0)
// 					{
// 						// Serial.println("Protocol error");
// 						strength = 0;
// 						// break
// 					}
// 					statebyte = statebyte + 1;

// 					if (statebyte == 2){
// 						pulse = (bit[6] * 64) + (bit[5] * 32) + (bit[4] * 16) + (bit[3] * 8) + (bit[2] * 4) + (bit[1] * 2) + bit[0];
// 					}
// 					if (statebyte == 3){
// 						pulse = pulse + (bit[7] * 128);
// 						// Serial.print("Pulse: ");
// 						Serial.print(pulse);
// 						Serial.print("\t");
// 						Serial.println(lpf.input(pulse));
// 					}

// 					if (statebyte == 5)
// 						statebyte = 0;
// 				}
// 			}
// 		}
// 	}
// }

void getBitsFromByte(byte b, int bit[8])
{

	bit[7] = (b & 128) >> 7;
	bit[6] = (b & 64) >> 6;
	bit[5] = (b & 32) >> 5;
	bit[4] = (b & 16) >> 4;
	bit[3] = (b & 8) >> 3;
	bit[2] = (b & 4) >> 2;
	bit[1] = (b & 2) >> 1;
	bit[0] = (b & 1) >> 0;

	// for (int i = 7; i >= 0; i--)
	// {
	// 	Serial.print(bit[i]);
	// }
	// Serial.print('\n');
}
void package1(int bit[8])
{
	int signalStrength = map((bit[3]*8) + (bit[2]*4)  + (bit[1]*2) + bit[0],0,15, 0, 8);
	Serial.print("Signal Strength: ");
	Serial.println(signalStrength);
	
	if(bit[4] != 0){
		Serial.println("Signal: Not Found");
	}else{
		Serial.println("Signal: OK");
	}

	if(bit[5] != 0){
		Serial.println("Probe: Unplugged");
	}else{
		Serial.println("Probe: OK");
	}

	if(bit[6] == 1){
		Serial.println("Pulse Beep");
	}

	if(bit[7] != 0){
		Serial.println("In Sync");
	}else{
		Serial.println("Not in sync");
	}
	Serial.print('\n');
}

void package2(int bit[8]){
	
	int pleth = map((bit[6]*64) + (bit[5]*32)  + (bit[4]*16) + (bit[3]*8) + (bit[2]*4)  + (bit[1]*2) + bit[0], 0,127, 0, 100);
	
	Serial.print("Pleth: ");
	Serial.println(pleth);
	
	if(bit[7] != 0){
		Serial.println("Not in Sync");
	}else{
		Serial.println("In sync");
	}
}

void package3(int bit[8]){
	
	int bargraph = (bit[3]*8) + (bit[2]*4)  + (bit[1]*2) + bit[0];
	
	Serial.print("Bargraph: ");
	Serial.println(bargraph);
	
	if(bit[4] != 0){
		Serial.println("Finger: None");
	}else{
		Serial.println("Finger: OK");
	}

	if(bit[5] != 0){
		Serial.println("Pulse: Research ");
	}else{
		Serial.println("Pulse: OK");
	}

	if(bit[7] != 0){
		Serial.println("Not in Sync");
	}else{
		Serial.println("In sync");
	}


}

void package4(int bit[8]){
	int pulse = (bit[6]*64) + (bit[5]*32)  + (bit[4]*16) + (bit[3]*8) + (bit[2]*4)  + (bit[1]*2) + bit[0];
	
	Serial.print("Pulse Rate: ");
	Serial.println(pulse);
	
	if(bit[7] != 0){
		Serial.println("Not in Sync");
	}else{
		Serial.println("In sync");
	}
}

void package5(int bit[8]){
	int spo2 = (bit[6]*64) + (bit[5]*32)  + (bit[4]*16) + (bit[3]*8) + (bit[2]*4)  + (bit[1]*2) + bit[0];
	
	Serial.print("SpO2: ");
	Serial.println(spo2);
	
	if(bit[7] != 0){
		Serial.println("Not in Sync");
	}else{
		Serial.println("In sync");
	}
}