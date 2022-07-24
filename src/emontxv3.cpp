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

// ArduinoJson5 code, replace if needed
  //StaticJsonBuffer<150> jsonBuffer;
  //JsonObject& root = jsonBuffer.createObject();
  //char msg[150];

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

  // Setup Mqtt connection
  /*PubSubClient client(espClient);
  client.setServer(MQTT_SERVER, MQTT_PORT);
  if (!client.connected()) {
      mqtt_reconnect(client, DEBUG);
  }
*/
  //IPAddress ip = WiFi.localIP();
  //char buf[60];
  //sprintf(buf, "%s @ IP:%d.%d.%d.%d SSID: %s", APPNAME, WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3], ssid );
  //Serial.println(buf);
  //root["status"] = buf;
  //root.printTo((char*)msg, root.measureLength() + 1);
  //client.publish(mqtt_debug_topic, msg);
  
  bmp180();
  bh1750fvi();
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
