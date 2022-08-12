#ifndef MQTT_FUNCTIONS_H_INCLUDED
#define MQTT_FUNCTIONS_H_INCLUDED
    #include <Arduino.h>
    #include <AsyncMqttClient.h>
    #include <Ticker.h>
    #include <emontxv3.h>
    #include <settings.h>

#define MQTT_PUB_TEST  "MQTT TEST"


    void mqtt_reconnect(PubSubClient pubsub, WiFiClient espClient, uint8_t debugflag);
    void mqtt_setup();
    void connectToMqtt();
    void onMqttConnect(bool sessionPresent);
    void onMqttDisconnect(AsyncMqttClientDisconnectReason reason);
    void onMqttPublish(uint16_t packetId);
    void mqtt_publish();

#endif