#ifndef OXYMETERY_H
#define OXYMETRY_H

#include <Arduino.h>
#include <SoftwareSerial.h>

class OxymetryData
{
private:
    SoftwareSerial* swSerial;
    void getBitsFromByte(byte, int*);
public:
    OxymetryData(SoftwareSerial* swSer);
    void begin(int32_t);
    void read();
    ~OxymetryData();
};

#endif