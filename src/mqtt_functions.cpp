#include <mqtt_functions.h>

//WiFiClient espClient;
//PubSubClient pubsub(espClient);
  AsyncMqttClient mqttClient;
Ticker mqttReconnectTimer;

void mqtt_init()
{

//  wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
//  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);
  Serial.println("MQTT init");


}

void connectToMqtt() {
  Serial.println("Connecting to MQTT...");
  mqttClient.connect();
}

void publishMqtt(char *data) {
  uint16_t packetIdPub = mqttClient.publish("test/lol", 0, true, "data");
  Serial.print("Publishing at QoS 0, packetId: ");
  Serial.println(packetIdPub);
}

void onMqttConnect(bool sessionPresent) {
  Serial.println("Connected to MQTT.");
  Serial.print("Session present: ");
  Serial.println(sessionPresent);
  /*uint16_t packetIdSub = mqttClient.subscribe("test/lol", 2);
  Serial.print("Subscribing at QoS 2, packetId: ");
  Serial.println(packetIdSub);
  mqttClient.publish("test/lol", 0, true, "test 1");
  Serial.println("Publishing at QoS 0");
  uint16_t packetIdPub1 = mqttClient.publish("test/lol", 1, true, "test 2");
  Serial.print("Publishing at QoS 1, packetId: ");
  Serial.println(packetIdPub1);
  */
  uint16_t packetIdPub2 = mqttClient.publish("test/lol", 2, true, "test 3");
  Serial.print("Publishing at QoS 2, packetId: ");
  Serial.println(packetIdPub2);
    char data[] = "Hello World!";

  publishMqtt(data);
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println("Disconnected from MQTT.");

  if (WiFi.isConnected()) {
    mqttReconnectTimer.once(2, connectToMqtt);
  }
}

void onMqttSubscribe(uint16_t packetId, uint8_t qos) {
  Serial.println("Subscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
  Serial.print("  qos: ");
  Serial.println(qos);
}

void onMqttUnsubscribe(uint16_t packetId) {
  Serial.println("Unsubscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}

void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
  Serial.println("Publish received.");
  Serial.print("  topic: ");
  Serial.println(topic);
  Serial.print("  qos: ");
  Serial.println(properties.qos);
  Serial.print("  dup: ");
  Serial.println(properties.dup);
  Serial.print("  retain: ");
  Serial.println(properties.retain);
  Serial.print("  len: ");
  Serial.println(len);
  Serial.print("  index: ");
  Serial.println(index);
  Serial.print("  total: ");
  Serial.println(total);
}

void onMqttPublish(uint16_t packetId) {
  Serial.println("Publish acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}


/*
//void mqtt_reconnect(PubSubClient pubsub, WiFiClient espClient, uint8_t debugflag) {
  void mqtt_reconnect(WiFiClient esp_client) {
  pubsub.setServer(MQTT_SERVER, MQTT_PORT);
  pubsub.setClient(espClient);

  while (!pubsub.connected()) {
    Serial.print("Attempting MQTT connection...");
     pubsub.setKeepAlive( 90 ); 
    // Attempt to connect
    //if (pubsub.connect(APPNAME, MQTT_USERNAME, MQTT_PASSWORD)) {
      if (pubsub.connect(APPNAME)) {
        #ifdef DEBUG
      
          String temp = "Connected to Mqtt broker as " + String(APPNAME);
          Serial.println(temp);
          //StaticJsonBuffer<150> jsonBuffer;
          //JsonObject& root = jsonBuffer.createObject();
          char msg[150];
          sprintf(msg, "Hello World!");
          //root["status"] = temp;
          //root.printTo((char*)msg, root.measureLength() + 1);
          pubsub.publish(MQTT_DEBUG_TOPIC, msg);
          Serial.println("Published test message");
        #endif  

    } else {
      Serial.print("Mqtt connection failed, rc=");
      Serial.print(pubsub.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
*/
