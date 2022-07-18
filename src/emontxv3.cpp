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

#define DEBUG 1

const char* ssid = "BrandstorpWifi";
const char* password = "Brandstorp";

//#define MQTT_USERNAME "emonpi"     
//#define MQTT_PASSWORD "emonpimqtt2016"  
const char* mqtt_server = "192.168.1.190";
char* mqtt_status_topic = "emontxv3/values";
char* mqtt_debug_topic = "emontxv3/debug";
char* mqtt_error_topic = "emontxv3/error";

#include <ESP8266WiFi.h>
#include <Wire.h>
// OTA
#include <ArduinoOTA.h>
// Json
#include <ArduinoJson.h>
// Mqtt
#include <PubSubClient.h>
WiFiClient espClient;
PubSubClient client(espClient);

// Light sensor
#include <BH1750FVI.h>

#include "SparkFunHTU21D.h"

// Pressure & temp
#include <SFE_BMP180.h>
SFE_BMP180 pressure;

#define ALTITUDE 54.0 // Altitude of Såtenäs (my location) in meters
char status;
double T,P,p0,a;
int iBMPtemp, iBMPpres;

// Light 
uint16_t lux;

// Humidity/temperature
HTU21D htu21d;
float humd, humt;
int ihumd, ihumt;

// ADC
#include <Adafruit_ADS1X15.h>
Adafruit_ADS1115 ads;

// I2C comms
const int sclPin = D5;
const int sdaPin = D4;
uint8_t ADDRESSPIN = 0;

const char compile_date[] = __DATE__ " " __TIME__;


// See https://github.com/PeterEmbedded/BH1750FVI/blob/master/src/BH1750FVI.h
BH1750FVI::eDeviceAddress_t DEVICEADDRESS = BH1750FVI::k_DevAddress_L;
BH1750FVI::eDeviceMode_t DEVICEMODE = BH1750FVI::k_DevModeOneTimeLowRes;
BH1750FVI LightSensor(ADDRESSPIN, DEVICEADDRESS, DEVICEMODE);

#define appname "emontxv3"

void setup() {
  if (!ads.begin()) {
    Serial.println("Failed to initialize ADS.");
    while (1);
  }

// ArduinoJson5 code, replace if needed
  //StaticJsonBuffer<150> jsonBuffer;
  //JsonObject& root = jsonBuffer.createObject();
  //char msg[150];
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println(F("Connection Failed! Rebooting..."));
    delay(5000);
    ESP.restart();
  }
  WiFi.hostname(appname);
  #ifdef DEBUG
    Serial.begin(115200);
    Serial.println("emontxv3");

    i2cscan();

    Serial.print(F("IP address: "));
    Serial.println(WiFi.localIP());

  #endif

// Enable OTA 
  // Hostname 
  ArduinoOTA.setHostname(appname);
    
  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  #ifdef DEBUG
    Serial.print("Over The Air programming enabled, port: ");
    Serial.println(appname);
  #endif

  // Setup Mqtt connection
  client.setServer(mqtt_server, 1883);
  if (!client.connected()) {
      reconnect();
  }

  IPAddress ip = WiFi.localIP();
  char buf[60];
  sprintf(buf, "%s @ IP:%d.%d.%d.%d SSID: %s", appname, WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3], ssid );
  Serial.println(buf);
  root["status"] = buf;
  root.printTo((char*)msg, root.measureLength() + 1);
  client.publish(mqtt_debug_topic, msg);
  
  bmp180();
  bh1750fvi();
  read_htu21d(); 

  ads.begin();
  int16_t adc0;  // we read from the ADC, we have a sixteen bit integer as a result
  adc0 = ads.readADC_SingleEnded(0);
  //Voltage = (adc0 * 0.1875)/1000;

  // Send values
  root["airPressure"] = iBMPpres;
  root["Light"] = lux;
  root["Humidity"] = ihumd;
  root["Temperature"] = ihumt;

  
  root.printTo((char*)msg, root.measureLength() + 1);
  client.publish(mqtt_debug_topic, msg);

  
}

void loop() {
  ArduinoOTA.handle();
}
