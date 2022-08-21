#include <mqtt_functions.h>

WiFiClient espClient;
PubSubClient mqtt_client(espClient);

void mqtt_init()
{
  Serial.println("MQTT init");

  mqtt_client.setServer(MQTT_SERVER, MQTT_PORT);
  if (!mqtt_client.connected()) {
    mqtt_reconnect();
  }
  #ifdef DEBUG  
    char msg[150];
    sprintf(msg, "Hello Moon!");
    mqtt_client.publish(MQTT_DEBUG_TOPIC, msg);
    mqtt_client.loop();
  #endif  
}

void mqtt_reconnect() 
{
  while (!mqtt_client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (mqtt_client.connect(APPNAME)) {
        #ifdef DEBUG  
          String temp = "Connected to Mqtt broker as " + String(APPNAME);
          Serial.println(temp);
          char msg[150];
          sprintf(msg, "Hello World!");
          mqtt_client.publish(MQTT_DEBUG_TOPIC, msg);
            mqtt_client.loop();
          Serial.println("Published test message");
        #endif  

    } else {
      Serial.print("Mqtt connection failed, rc=");
      Serial.print(mqtt_client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
void mqtt_send(const char *txt)
{
  #ifdef DEBUG  
    Serial.print("Send via Mqtt: ");
    Serial.print(txt);
  #endif
  mqtt_client.publish(MQTT_STATUS_TOPIC, txt);
  mqtt_client.loop();
  delay(5);

}
void mqtt_test()
{
  char msg[150];
  sprintf(msg, "Mqtt test message");
  mqtt_client.publish(MQTT_DEBUG_TOPIC, msg);
  mqtt_client.loop();
  delay(5);   // This is important, won't work without it!
  Serial.println("Published test message 2");
}