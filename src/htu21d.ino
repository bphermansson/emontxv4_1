void read_htu21d() {
    // Init humidity sensor
    #ifdef DEBUG
      //Serial.println("HTU21 init");
    #endif  
    htu21d.begin();
    // Read humidity sensor
    humd = htu21d.readHumidity();
    ihumd = (int) humd;

    // Read temp sensor
    humt = htu21d.readTemperature();
    ihumt = (int) humt;
    
    #ifdef DEBUG
      Serial.print("Humidity = ");
      Serial.print(ihumd);
      Serial.println("%");
      Serial.print("HTU21D temp = ");
      Serial.print(ihumt);
      Serial.println("C");
    #endif

}
