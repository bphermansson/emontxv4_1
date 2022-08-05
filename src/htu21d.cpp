#include "SparkFunHTU21D.h"
#include <htu21d.h>

// Humidity/temperature
HTU21D htu21d;
float humd, humt;
int ihumd, ihumt;

void read_htu21d(htuvalues *htuvalues) {
    // Init humidity sensor
    #ifdef DEBUG
      //Serial.println("HTU21 init");
    #endif  
    htu21d.begin();
    // Read humidity sensor
    humd = htu21d.readHumidity();
    ihumd = (int) humd*10;  

    // Read temp sensor
    humt = htu21d.readTemperature();
    ihumt = (int) humt*10;
    
    htuvalues->temp = humt;
    htuvalues->humidity = humd;

    #ifdef DEBUG
      Serial.print("HTU21D humidity = ");
      Serial.print(humd);
      Serial.print(" - ");
      Serial.print(ihumd);
      Serial.println("%");
      Serial.print("HTU21D temp = ");
      Serial.print(humt);
      Serial.print(" - ");
      Serial.print(ihumt);
      Serial.println("C");
    #endif

}
