#include <mqtt_reconnect.h>

void mqtt_reconnect(PubSubClient client, uint8_t debugflag) {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("appname", MQTT_USERNAME, MQTT_PASSWORD)) {
    #ifdef DEBUG
      
        String temp = "Connected to Mqtt broker as " + String(APPNAME);
        Serial.println(temp);
        //StaticJsonBuffer<150> jsonBuffer;
        //JsonObject& root = jsonBuffer.createObject();
        char msg[150];
        //root["status"] = temp;
        //root.printTo((char*)msg, root.measureLength() + 1);
        client.publish(MQTT_DEBUG_TOPIC, msg);
    #endif  
      
    } else {
      Serial.print("Mqtt connection failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
