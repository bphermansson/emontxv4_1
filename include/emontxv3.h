#ifndef EMONTXV3_H_INCLUDED
#define EMONTXV3_H_INCLUDED
    #include <ArduinoJson.h>
    #define MY_JSON_OBJECT_SIZE 100
    
    #include <ESP8266WiFi.h>
    #include <PubSubClient.h>

    typedef struct
    {
        float battery_voltage;
        float solar_cell_voltage;
        float uvlight_sensor_voltage;
        float battery_current;           // Charge or disharge
    } advalues;
#endif
