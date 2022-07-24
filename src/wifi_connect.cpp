#include "wifi_connect.h"

void connectWifi(char *wifidata) {
  WiFiClient espClient;
  delay(2000);
  Serial.println("Connect to Wifi...");
  WiFi.persistent(false);  // Do not write Wifi settings to flash
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFISSID, WIFIPASSWORD);

  uint8_t connAttempts = 0;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.printf(". %d \n", connAttempts);
    connAttempts++;
    if (connAttempts>23) {
      Serial.println("WiFi connection error, check your settings.");    
      Serial.println((String)"In settings ssid="+WIFISSID+" and pass="+WIFIPASSWORD);      
      while(connAttempts > 25) {
        yield();
      }
    }
    delay(2000);
  }
  delay(3000);
    
    IPAddress ip = WiFi.localIP();
    Serial.print("Ip in con: ");
    Serial.println(ip);
    sprintf(wifidata, "%u.%u.%u.%u", ip[0], ip[1], ip[2], ip[3]);
}