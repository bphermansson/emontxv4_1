// Light sensor
#include <BH1750_read.h>

void bh1750fvi(uint8_t *luxPtr) {

  BH1750FVI myBH1750(BH1750_DEFAULT_I2CADDR, BH1750_CONTINUOUS_HIGH_RES_MODE_2, BH1750_SENSITIVITY_DEFAULT, BH1750_ACCURACY_DEFAULT);

  while (myBH1750.begin(SDAPIN, SCLPIN) != true)
  {
    Serial.println(F("ROHM BH1750FVI is not present")); //(F()) saves string to flash & keeps dynamic memory free
    delay(5000);
  }

  float l = myBH1750.readLightLevel();
  *luxPtr = round(l);
  #ifdef DEBUG
    Serial.printf("Lightlevel: %d lux\n", *luxPtr);  
  #endif  
}