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
#include <calculateCRC32.h>

// Settings
#include <settings.h>

// ADC
#include <Adafruit_ADS1X15.h>
Adafruit_ADS1115 ads;

const char compile_date[] = __DATE__ " " __TIME__;

// Use this to store sensor values
uint8_t *measured_values_array;

int length = 10;

struct {
  uint32_t crc32;   // 4 bytes
  uint8_t channel;  // 1 byte,   5 in total
  uint8_t ap_mac[6];// 6 bytes, 11 in total
  uint8_t padding;  // 1 byte,  12 in total
} rtcData;

IPAddress ip( 192, 168, 1, 135 );
IPAddress gateway( 192, 168, 1, 1 ); 
IPAddress subnet( 255, 255, 255, 0 );
IPAddress dns(192,168,1,1);

void setup() {


/* -------- Borrowed code -------- */


  // https://gitlab.com/diy_bloke/verydeepsleep/blob/master/VeryDeepSleep.ino
  // we disable WiFi, coming from DeepSleep, as we do not need it right away
  WiFi.mode( WIFI_OFF );
  WiFi.forceSleepBegin();
  // Try to read WiFi settings from RTC memory
  bool rtcValid = false;
  if( ESP.rtcUserMemoryRead( 0, (uint32_t*)&rtcData, sizeof( rtcData ) ) ) {
    // Calculate the CRC of what we just read from RTC memory, but skip the first 4 bytes as that's the checksum itself.
    uint32_t crc = calculateCRC32( ((uint8_t*)&rtcData) + 4, sizeof( rtcData ) - 4 );
    if( crc == rtcData.crc32 ) {
      rtcValid = true;
    }
  }

// Now is the time to do stuff like reading sensors while the radio is still off
/* -------- End borrowed code -------- */

  #ifdef DEBUG
    Serial.begin(115200);
  #endif

  //char wd[100];
  //connectWifi(wd);

  #ifdef DEBUG
    Serial.printf("\n---emontxv4_1---\n\n");

    i2cscan(SDAPIN, SCLPIN);

   // Serial.print(F("Wifi connected, IP address: "));
   // Serial.println(wd);

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
    Serial.println("Read lux from BH1750");
  #endif
  
  uint8_t lux=0;
  uint8_t *luxPtr = &lux;
  bh1750fvi(luxPtr);

  #ifdef DEBUG
    Serial.printf("Got %d lux from reading BH1750.\n", *luxPtr);  
  #endif

  #ifdef DEBUG
    Serial.println("Read Humidity/temperature from HTU21D");
  #endif

  htuvalues *temphumPtr = (htuvalues*)malloc(sizeof(htuvalues));

  read_htu21d(temphumPtr); 

  #ifdef DEBUG
    Serial.printf("Got humidity %f from reading HTU21D.\n", temphumPtr->humidity);  
    Serial.printf("Got temp %f from reading HTU21D.\n", temphumPtr->temp);  
  #endif


  ads.begin();  // Start the I2C connected ADC (ADS1115, four channels, 16 bits)  
  // Here a voltage divider is used. 47k from battery to ADC in, 100k to ground.
  // 3.9V from battery, 2.64 to ADC. 3.9/2.64 = 1.4773. ADC value = 14050.
  uint16_t battery = ads.readADC_SingleEnded(0);
  float volts0 = ads.computeVolts(battery);
  float adjVolt = volts0*1.4773;
  
  #ifdef DEBUG
    Serial.printf("ADC reading: %d. Converted voltage: %f. Real voltage: %f\n", battery, volts0, adjVolt);  
  #endif

  uint16 solar;
  solar = ads.readADC_SingleEnded(1);
  uint16_t uvlight;
  uvlight = ads.readADC_SingleEnded(2);
  uint16_t solarcurrent;
  solarcurrent = ads.readADC_SingleEnded(3);

    Serial.printf("AD: %d %d %d\n", solar, uvlight, solarcurrent);  

  // Send values
  //root["airPressure"] = iBMPpres;
  //root["Light"] = lux;
  //root["Humidity"] = ihumd;
  //root["Temperature"] = ihumt;

  
  //root.printTo((char*)msg, root.measureLength() + 1);
//  client.publish(mqtt_debug_topic, msg);

  
/* -------- Borrowed code -------- */
 //Switch Radio back On

  WiFi.forceSleepWake();
  delay( 1 );
  // Disable the WiFi persistence.  The ESP8266 will not load and save WiFi settings unnecessarily in the flash memory.
  WiFi.persistent( false );

  // Bring up the WiFi connection
  WiFi.mode( WIFI_STA );
  WiFi.config( ip,dns, gateway, subnet );
  //-----------Now we replace the normally used "WiFi.begin();" with a precedure using connection data stored by us
  if( rtcValid ) {
  // The RTC data was good, make a quick connection
    WiFi.begin( WIFISSID, WIFIPASSWORD, rtcData.channel, rtcData.ap_mac, true );
  }
else {
  // The RTC data was not valid, so make a regular connection
  WiFi.begin( WIFISSID, WIFIPASSWORD );
}
//WiFi.begin( WLAN_SSID, WLAN_PASSWD );
//------now wait for connection
  int retries = 0;
  int wifiStatus = WiFi.status();
  while( wifiStatus != WL_CONNECTED ) {
    retries++;
    if( retries == 100 ) {
      // Quick connect is not working, reset WiFi and try regular connection
      WiFi.disconnect();
      delay( 10 );
      WiFi.forceSleepBegin();
      delay( 10 );
      WiFi.forceSleepWake();
      delay( 10 );
      WiFi.begin( WIFISSID, WIFIPASSWORD );
    } 
  if( retries == 600 ) {
    // Giving up after 30 seconds and going back to sleep
    WiFi.disconnect( true );
    delay( 1 );
    WiFi.mode( WIFI_OFF );
    ESP.deepSleep( SLEEPTIME, WAKE_RF_DISABLED );
    return; // Not expecting this to be called, the previous call will never return.
  }
  delay( 50 );
    wifiStatus = WiFi.status();
  }
  //---------
  #ifdef DEBUG

    Serial.println(" WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  #endif  

  //-----
  // Write current connection info back to RTC
  rtcData.channel = WiFi.channel();
  memcpy( rtcData.ap_mac, WiFi.BSSID(), 6 ); // Copy 6 bytes of BSSID (AP's MAC address)
  rtcData.crc32 = calculateCRC32( ((uint8_t*)&rtcData) + 4, sizeof( rtcData ) - 4 );
  ESP.rtcUserMemoryWrite( 0, (uint32_t*)&rtcData, sizeof( rtcData ) );
  //-------Now it's time to use your connection, e.g. by sending data
  //senData();//send data
  //----and go to back to sleep
  WiFi.disconnect( true );
  delay( 1 );
  // WAKE_RF_DISABLED to keep the WiFi radio disabled when we wake up
  
  ESP.deepSleep( SLEEPTIME, WAKE_RF_DISABLED );
  /* -------- End borrowed code -------- */

}

void loop() {
  //ArduinoOTA.handle();
}
