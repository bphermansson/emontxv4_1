#include <mqtt_functions.h>

AsyncMqttClient mqtt_client;
Ticker mqttReconnectTimer;

void mqtt_setup()
{
  mqtt_client.onConnect(onMqttConnect);
  mqtt_client.onDisconnect(onMqttDisconnect);
  mqtt_client.onPublish(onMqttPublish);
  mqtt_client.setServer(MQTT_SERVER, MQTT_PORT);
}

void mqtt_publish()
{
     uint16_t packetIdPub1 = mqtt_client.publish(MQTT_PUB_TEST, 1, true, "Test");
    Serial.printf("Publishing on topic %s at QoS 1, packetId: %i", MQTT_PUB_TEST, packetIdPub1);
    Serial.printf("Message: %s \n", "Test");
}


void connectToMqtt() {  // TODO: Rename this!
  Serial.println("Connecting to MQTT...");
  mqtt_client.connect();
}

void onMqttConnect(bool sessionPresent) {
  Serial.println("Connected to MQTT.");
  Serial.print("Session present: ");
  Serial.println(sessionPresent);
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println("Disconnected from MQTT.");

  if (WiFi.isConnected()) {
    mqttReconnectTimer.once(2, connectToMqtt);
  }
}

void onMqttPublish(uint16_t packetId) {
  Serial.print("Publish acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}


// Old function remove!?
void mqtt_reconnect(PubSubClient pubsub, WiFiClient espClient, uint8_t debugflag) {
  pubsub.setServer(MQTT_SERVER, MQTT_PORT);
  pubsub.setClient(espClient);

  while (!pubsub.connected()) {
    Serial.print("Attempting MQTT connection...");
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
