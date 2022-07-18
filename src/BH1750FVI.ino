void bh1750fvi() {
  LightSensor.begin();  
  delay(100);
  lux = LightSensor.GetLightIntensity();
  #ifdef DEBUG
    Serial.printf("Light: %d lux", lux);
    Serial.println();
  #endif  
  delay(250);
}
