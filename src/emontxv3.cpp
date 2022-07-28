/* 
 Sensors used:
  -BMP180, air pressure/temperature, I2C.
     Code uses Sparkfuns lib for BMP180, 
     https://learn.sparkfun.com/tutorials/bmp180-barometric-pressure-sensor-hookup-?_ga=1.148112447.906958391.1421739042
  -HTU21D, humidity/temperature, I2C. Mounted at a good(external) location.
     https://learn.sparkfun.com/tutorials/htu21d-humidity-sensor-hookup-guide/htu21d-overview
  -BH1750FVI, light, I2C.
  -ADS1115, ADC, I2C.  
*/  


// External libs
#include <ArduinoJson.h>

// Internal functions
#include <i2cscan.h>
#include <htu21d.h>
#include <bmp180.h>
#include <BH1750_read.h>
#include <mqtt_reconnect.h>
#include <wifi_connect.h>

// Settings
#include <settings.h>

// ADC
#include <Adafruit_ADS1X15.h>
Adafruit_ADS1115 ads;

const char compile_date[] = __DATE__ " " __TIME__;

// Use this to store sensor values
uint8_t *measured_values_array;
//measured_values_array = NULL;
int length = 10;
//measured_values_array = malloc(length * sizeof(int));

void setup() {
  #ifdef DEBUG
    Serial.begin(115200);
  #endif

  char wd[100];
  connectWifi(wd);

  #ifdef DEBUG
    Serial.printf("\n---emontxv3---\n\n");

    i2cscan(SDAPIN, SCLPIN);

    Serial.print(F("Wifi connected, IP address: "));
    Serial.println(wd);

  #endif

  if (!ads.begin()) {
      Serial.println("Failed to initialize ADS.");
    while (1);
  }
  else
  {
    #ifdef DEBUG
      Serial.println("Initialized ADS.");
    #endif
  }
  
  #ifdef DEBUG
      Serial.println("Read BMP180");
  #endif
  
  bmpvalues *temppresPtr = (bmpvalues*)malloc(sizeof(bmpvalues));
  
  bmp180(temppresPtr);  
  #ifdef DEBUG
    Serial.println("Result from BMP180");
    Serial.print("BMP180 temp: ");
    Serial.println(temppresPtr->temp);
    Serial.print("BMP180 absolute pressure: ");
    Serial.println(temppresPtr->abspressure);
    Serial.print("BMP180 pressure: ");
    Serial.println(temppresPtr->pressure);
  #endif  

  #ifdef DEBUG
    Serial.println("Init bh1750");
  #endif
  bh1750fvi();
  #ifdef DEBUG
    Serial.println("Init htu21d");
  #endif
  read_htu21d(); 

  ads.begin();
  int16_t adc0;  // we read from the ADC, we have a sixteen bit integer as a result
  adc0 = ads.readADC_SingleEnded(0);
  //Voltage = (adc0 * 0.1875)/1000;

  // Send values
  //root["airPressure"] = iBMPpres;
  //root["Light"] = lux;
  //root["Humidity"] = ihumd;
  //root["Temperature"] = ihumt;

  
  //root.printTo((char*)msg, root.measureLength() + 1);
//  client.publish(mqtt_debug_topic, msg);

  
}

void loop() {
  //ArduinoOTA.handle();
}
