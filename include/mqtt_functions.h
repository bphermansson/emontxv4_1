#ifndef MQTT_FUNCTIONS_H_INCLUDED
#define MQTT_FUNCTIONS_H_INCLUDED
    #include <Arduino.h>
    #include <PubSubClient.h>
    #include <emontxv3.h>
    #include <settings.h>
    
    #include <Ticker.h>
    #include <AsyncMqttClient.h>
    #define MQTT_PUB_TEST  "MQTT TEST"


    //void mqtt_reconnect(PubSubClient pubsub, WiFiClient espClient, uint8_t debugflag);
    void mqtt_init();
    void publishMqtt(char *data);
    void connectToMqtt();
    void onMqttConnect(bool sessionPresent);
    void onMqttDisconnect(AsyncMqttClientDisconnectReason reason);
    void onMqttSubscribe(uint16_t packetId, uint8_t qos);
    void onMqttUnsubscribe(uint16_t packetId);
    void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);
    void onMqttPublish(uint16_t packetId);


    void mqtt_reconnect(WiFiClient esp_client);


#endif