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
    
    int spo2;
    int signalStrength;
    int pulseRate;
    int pleth;

    SoftwareSerial* swSerial;
    void clearBuffer();
    void getBitsFromByte(byte, int*);
    void packet1(int bit[8]);
    void packet2(int bit[8]);
    void packet3(int bit[8]);
    void packet4(int bit[8]);
    void packet5(int bit[8]);
    void update(int,int,int);
    void updatePleth(int);
    void cleanRead();
    
public:
    OxiParser(SoftwareSerial* swSer);
    void begin(int32_t);
    void begin();
    void read();
    int getSpo2();
    int getSignalStrength();
    int getPulseRate();
    int getPleth();

    ~OxiParser();
};



#endif