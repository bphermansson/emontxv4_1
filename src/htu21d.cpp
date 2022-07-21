#ifndef HTU21_H_INCLUDED
#define HTU21_H_INCLUDED

#include "SparkFunHTU21D.h"

// Humidity/temperature
HTU21D htu21d;
float humd, humt;
int ihumd, ihumt;

void read_htu21d() {
    // Init humidity sensor
    #ifdef DEBUG
      //Serial.println("HTU21 init");
    #endif  
    htu21d.begin();
    // Read humidity sensor
    humd = htu21d.readHumidity();
    ihumd = (int) humd;

    // Read temp sensor
    humt = htu21d.readTemperature();
    ihumt = (int) humt;
    
    #ifdef DEBUG
      Serial.print("Humidity = ");
      Serial.print(ihumd);
      Serial.println("%");
      Serial.print("HTU21D temp = ");
      Serial.print(ihumt);
      Serial.println("C");
    #endif

}
#endif