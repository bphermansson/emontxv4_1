#ifndef MQTT_RECONNECT_H_INCLUDED
#define MQTT_RECONNECT_H_INCLUDED
    #include <Arduino.h>
    #include <emontxv3.h>
    #include <settings.h>
    void mqtt_reconnect(PubSubClient pubsub, WiFiClient espClient, uint8_t debugflag);
#endif