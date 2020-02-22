#ifndef OXIPARSER_H
#define OXIPARSER_H

#include <Arduino.h>
#include <SoftwareSerial.h>



enum Search {
    SEARCH,
    NOT_FOUND,
    FOUND,
};



class OxiParser
{
private:
      
    
    byte spo2;
    byte signalStrength;
    byte pulseRate;
    byte pleth;
    byte beep;
    void (*_callBackReadRaw)(byte);
    
    void (*_callBackSpo2)(byte);
    void (*_callBackSignalStrength)(byte);
    void (*_callBackPulseRate)(byte);
    void (*_callBackPleth)(byte);
    void (*_callBackBeep)(byte);
   
    

    SoftwareSerial* swSerial;
    void clearBuffer();
    
    void update(byte,byte,byte);
    void updatePleth(byte);
    void cleanRead();

public:

    OxiParser(SoftwareSerial* swSer);

    void begin(int32_t);
    void begin();
    void loop();
    void readRaw( void (*)(byte));
    void readSignalStrength( void(*)(byte));
    void readPulseRate( void(*)(byte));
    void readPleth( void(*)(byte));
    void readSpo2( void(*)(byte));
    void readBeep( void(*)(byte));
    void getBitsFromByte(byte, int*);

    ~OxiParser();
};



#endif