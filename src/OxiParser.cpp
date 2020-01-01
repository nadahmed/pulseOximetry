#include <OxiParser.h>

OxiParser::OxiParser(SoftwareSerial* swSer)
{
    swSerial = swSer;
    
}

void OxiParser::begin(int32_t BAUD_RATE){
    
    swSerial->begin(BAUD_RATE, SWSERIAL_8N1);
}

void OxiParser::begin(){
    
    swSerial->begin(56700, SWSERIAL_8N1);
}

void OxiParser::read(){
    
    static Search state = SEARCH;
	static unsigned long starttime = millis();

    switch(state){
        case SEARCH:
            if (swSerial->available() < 5){
                if ((millis() - starttime) > 300){  // Not data was available for a while
                    state = NOT_FOUND;
                }
            }else{
                state = FOUND;
            }
        
        case NOT_FOUND:
            clearBuffer();
            state = SEARCH;    //Search Again
            starttime = millis();   //Reset timer
        
        case FOUND:
            cleanRead();
            clearBuffer();

            state = SEARCH;
            starttime = millis();
    }
}

void OxiParser::clearBuffer(){
    while (swSerial->available() > 0)   //Empty the Serial buffer incase of incomplete packets
        {
            swSerial->read();
        }
}

void OxiParser::cleanRead(){
    uint8_t packet[5];
    int dat;
    dat = swSerial->read();
    if((dat & 0x80) >0){
        packet[0]=dat;
        for (int i=1; i<5; i++){
            dat = swSerial->read();
            if((dat & 0x80) == 0){
                packet[i]=dat;
            }
        }
    }

    int _spo2 = packet[4];
    int _pulseRate = packet[3] | ((packet[2] & 0x40) << 1);
    int _signalStrength = packet[0] & 0x0f;
    updatePleth(packet[1]);
    if(_spo2 != spo2 || _pulseRate != pulseRate || _signalStrength != signalStrength)
        {
            update(_spo2,_pulseRate,_signalStrength);
        }

}

void OxiParser::update(int _spo2,int _pulseRate,int _signalStrength){
    spo2 = _spo2;
    pulseRate = _pulseRate;
    signalStrength = _signalStrength;
}

void OxiParser::updatePleth(int _pleth){
    pleth=_pleth;
}

int OxiParser::getPulseRate(){
    return pulseRate;
}

int OxiParser::getSpo2(){
    return spo2;
}

int OxiParser::getSignalStrength(){
    return signalStrength;
}

int OxiParser::getPleth(){
    return pleth;
}

void OxiParser::getBitsFromByte(uint8_t _byte, int bit[8])
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


void OxiParser::packet1(int bit[8])
{
	int signalStrength = map((bit[3]*8) + (bit[2]*4)  + (bit[1]*2) + bit[0],0,15, 0, 8);
	// Serial.print("Signal Strength: ");
	// Serial.println(signalStrength);
	
	if(bit[4] != 0){
		// Serial.println("Signal: Not Found");
	}else{
		// Serial.println("Signal: OK");
	}

	if(bit[5] != 0){
		// Serial.println("Probe: Unplugged");
	}else{
		// Serial.println("Probe: OK");
	}

	if(bit[6] == 1){
		// Serial.println("Pulse Beep");
	}

	if(bit[7] != 0){
		// Serial.println("In Sync");
	}else{
		// Serial.println("Not in sync");
	}
	// Serial.print('\n');
}

void OxiParser::packet2(int bit[8]){
	
	int pleth = map((bit[6]*64) + (bit[5]*32)  + (bit[4]*16) + (bit[3]*8) + (bit[2]*4)  + (bit[1]*2) + bit[0], 0,127, 0, 100);
	
	// Serial.print("Pleth: ");
	// Serial.print(pleth);
	
	
	if(bit[7] != 0){
		// Serial.println("Not in Sync");
	}else{
		// Serial.println("In sync");
	}
}

void OxiParser::packet3(int bit[8]){
	
	int bargraph = (bit[3]*8) + (bit[2]*4)  + (bit[1]*2) + bit[0];
	
	// Serial.print("Bargraph: ");
	// Serial.print(bargraph);
	

	if(bit[4] != 0){
		// Serial.println("Finger: None");
	}else{
		// Serial.println("Finger: OK");
	}

	if(bit[5] != 0){
		// Serial.println("Pulse: Research ");
	}else{
		// Serial.println("Pulse: OK");
	}

	if(bit[7] != 0){
		// Serial.println("Not in Sync");
	}else{
		// Serial.println("In sync");
	}


}

void OxiParser::packet4(int bit[8]){
	int pulse = (bit[6]*64) + (bit[5]*32)  + (bit[4]*16) + (bit[3]*8) + (bit[2]*4)  + (bit[1]*2) + bit[0];
	
	// Serial.print("Pulse Rate: ");
	Serial.print(pulse);
	
	if(bit[7] != 0){
		// Serial.println("Not in Sync");
	}else{
		// Serial.println("In sync");
	}
}

void OxiParser::packet5(int bit[8]){
	int spo2 = (bit[6]*64) + (bit[5]*32)  + (bit[4]*16) + (bit[3]*8) + (bit[2]*4)  + (bit[1]*2) + bit[0];
	
	// Serial.print("SpO2: ");
	Serial.print(spo2);
	
	
	if(bit[7] != 0){
		// Serial.println("Not in Sync");
	}else{
		// Serial.println("In sync");
	}
}


OxiParser::~OxiParser()
{
}