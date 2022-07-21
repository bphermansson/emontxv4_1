#ifndef MQTT_RECONNECT_H_INCLUDED
#define MQTT_RECONNECT_H_INCLUDED
    // Mqtt
    #include <PubSubClient.h>
    #include <settings.h>
    void mqtt_reconnect(PubSubClient client, uint8_t debugflag);
#endif