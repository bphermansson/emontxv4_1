#ifndef MQTT_FUNCTIONS_H_INCLUDED
#define MQTT_FUNCTIONS_H_INCLUDED
    #include <Arduino.h>
    #include <PubSubClient.h>
    #include <emontxv3.h>
    #include <settings.h>
    
    //#define MQTT_PUB_TEST  "MQTT TEST"
    void mqtt_init();
    void mqtt_reconnect();
    void mqtt_test();
#endif