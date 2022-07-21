#include <i2cscan.h>

void i2cscan(int sda, int scl)
{
  Wire.begin(sda, scl);
  Serial.println("I2C Scanner");

  uint8_t error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for (address = 1; address < 127; address++)
  {
    // The i2c scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.print(address, HEX);
      Serial.println(" !");
      //Serial.println(address);
      switch(address) {
        case 35:  // 0x23
          Serial.println("BH1750FVI");
        break;
        case 64:  // 0x40
          Serial.println("HTU21D");
        break;
        case 72:  // 0x48
          Serial.println("ADS1115");
        break;
        case 119:  // 0x77
          Serial.println("BMP180");
        break;
      }

      nDevices++;
    }
    else if (error == 4)
    {
      Serial.print("Unknown error at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  }
  else {
    Serial.println("I2C scanning done.\n");
  }

  delay(2000);
}
