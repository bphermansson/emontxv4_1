#ifndef WIFI_CONNECT_H_INCLUDED
#define WIFI_CONNECT_H_INCLUDED
    #include <Arduino.h>
    #include <ESP8266WiFi.h>
    #include <settings.h>

    typedef struct
    {
        char ipaddress;
    } wifidataarray;   

    //void connectWifi(wifidataarray *pwaddr);
    void connectWifi(char *pwaddr);
#endif
