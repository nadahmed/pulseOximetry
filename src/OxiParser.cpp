#include <OxiParser.h>

OxiParser::OxiParser(SoftwareSerial* swSer)
{
    swSerial = swSer;
    
}

void OxiParser::begin(int32_t BAUD_RATE){
    
    swSerial->begin(BAUD_RATE, SWSERIAL_8N1);
}

void OxiParser::begin(){
    
    swSerial->begin(57600, SWSERIAL_8N1);
}

void OxiParser:: readRaw( void (*callBackunction)(byte data)){
	_callBackReadRaw = callBackunction;
}
void OxiParser::loop(){
    // Serial.println("[OXIPARSER]: Read started!");
    static Search state = SEARCH;
	static unsigned long starttime = 0;

    switch(state){
        case SEARCH:
            if (swSerial->available() < 9){
				// Serial.println("[OXIPARSER]: Entered Serial Loop");
				// Serial.println(starttime);
				// Serial.println(swSerial->available());
				
                if ((millis() - starttime) > 2000){  // No data was available for a while
					
					// Serial.println("[OXIPARSER]: Not Found");
					state = NOT_FOUND;
                }
            }else{
				// Serial.println("[OXIPARSER]: Found");
                state = FOUND;
            }
        break;
        case NOT_FOUND:
            clearBuffer();
            state = SEARCH;    //Search Again
            starttime = millis();   //Reset timer
        break;
        case FOUND:
            cleanRead();
            // clearBuffer();

            state = SEARCH;
            starttime = millis();
			break;
    }
}

void OxiParser::clearBuffer(){
    while (swSerial->available() > 0)   //Empty the Serial buffer incase of incomplete packets
        {
            swSerial->read();
        }
}

void OxiParser::cleanRead(){
    byte packet[9];
    byte dat = swSerial->read();
	_callBackReadRaw(dat);
    // if((dat & 0x01) == 0x01){
        packet[0]=dat;
        dat = swSerial->read();
        if(dat == 0x01){
            packet[1] = dat;
            
            for (int i=2; i<9; i++){
                dat = swSerial->read();
                _callBackReadRaw(dat);
                // if((dat & 0x80) == 0){
                    packet[i]=dat;
                // }
            }


            



        }

    // }
byte _spo2 = packet[6];
            byte _pulseRate = packet[5]; // | ((packet[2] & 0x40) << 1);
            byte _signalStrength = packet[2] & 0x07;
            
            byte _beep = packet[2] & 0x40;

            updatePleth(packet[3]);
            
                // Serial.println(beep);
            _callBackBeep(_beep);
            
            // if(spo2 != 0x7F){
                spo2 = _spo2;
                _callBackSpo2(spo2);
            // } 
            if(_pulseRate != pulseRate && _pulseRate != 128){
                pulseRate = _pulseRate;
                _callBackPulseRate(pulseRate);
            } 
            if(_signalStrength != signalStrength){
                signalStrength = _signalStrength;
                _callBackSignalStrength(signalStrength);
            }
    

}


void OxiParser::updatePleth(byte _pleth){
	// if(_pleth != 256 && _pleth != 18){
    //     pleth = _pleth;
        _callBackPleth(_pleth);
    // }

}

void OxiParser::readBeep(void (*callBackFunction)(byte data)){
    _callBackBeep = callBackFunction;
}

void OxiParser::readPulseRate(void (*callBackFunction)(byte data)){
    _callBackPulseRate = callBackFunction;
}

void OxiParser::readSpo2(void (*callBackFunction)(byte data)){
    _callBackSpo2 = callBackFunction;
}

void OxiParser::readSignalStrength(void (*callBackFunction)(byte data)){
    _callBackSignalStrength = callBackFunction;
}

void OxiParser::readPleth(void (*callBackFunction)(byte data)){
    _callBackPleth = callBackFunction;
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


OxiParser::~OxiParser()
{
}