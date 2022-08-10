#include <mqtt_reconnect.h>

void mqtt_reconnect(PubSubClient pubsub, WiFiClient espClient, uint8_t debugflag) {
  pubsub.setServer(MQTT_SERVER, MQTT_PORT);
  pubsub.setClient(espClient);

  // Loop until we're reconnected
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
