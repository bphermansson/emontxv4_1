 
// Pressure & temp
#include <SFE_BMP180.h>
#include <bmp180.h>
#include <settings.h>
SFE_BMP180 pressure;
uint8_t iBMPtemp, iBMPpres;

#define ALTITUDE 54.0 // Altitude of Såtenäs (my location) in meters
char status;
double T,P,p0,a;

void bmp180(bmpvalues *temppresPtr) {
  //temppresPtr->temp = 1;  
  
  // Initialize the BMP180 (it is important to get calibration values stored on the device).
  if (pressure.begin()) {
    #ifdef DEBUG
      Serial.println("BMP180 init success");
    #endif 
    __asm__("nop\n\t");    
  }
  else {
    // Oops, something went wrong, this is usually a connection problem,
    // see the comments at the top of this sketch for the proper connections.
    #ifdef DEBUG
      Serial.println("BMP180 init fail\n\n");
    #endif  
  }
  // Air pressure, you must read the BMP180:s temp too.
 
  #ifdef DEBUG
    Serial.println("Measure BMP180 temp and air pressure");
  #endif  
  
  status = pressure.startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:
    delay(status);

    // Retrieve the completed temperature measurement:
    // Note that the measurement is stored in the variable T.
    // Function returns 1 if successful, 0 if failure.

    status = pressure.getTemperature(T);
    if (status != 0)
    {
      iBMPtemp = (int) T;
      // Print out the measurement:
      #ifdef DEBUG
        Serial.print("BMP180 temp: ");
        Serial.print(iBMPtemp);
        Serial.println("C");
      #endif
      // Convert to int
      //double T2=T*100; // Preserve decimals
      /*
      #ifdef DEBUG
        Serial.print("BMP180 temp as int:");
        Serial.println(iBMPtemp);
      #endif
      */
      // Start a pressure measurement:
      // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
      // If request is successful, the number of ms to wait is returned.
      // If request is unsuccessful, 0 is returned.

      status = pressure.startPressure(3);
      if (status != 0)
      {
        // Wait for the measurement to complete:
        delay(status);

        // Retrieve the completed pressure measurement:
        // Note that the measurement is stored in the variable P.
        // Note also that the function requires the previous temperature measurement (T).
        // (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
        // Function returns 1 if successful, 0 if failure.
        delay(1000);
        #ifdef DEBUG
           Serial.println("Get pressure value");
        #endif
        status = pressure.getPressure(P,T);
        if (status != 0)
        {
          #ifdef DEBUG
            Serial.print("Got p value: ");
            Serial.print(P);
            Serial.println(" mb/hPa");
          #endif

          // The pressure sensor returns abolute pressure, which varies with altitude.
          // To remove the effects of altitude, use the sealevel function and your current altitude.
          // This number is commonly used in weather reports.
          // Parameters: P = absolute pressure in mb, ALTITUDE = current altitude in m.
          // Result: p0 = sea-level compensated pressure in mb

          p0 = pressure.sealevel(P,ALTITUDE); 

          #ifdef DEBUG
            Serial.print("Relative (sea-level) pressure: ");
            Serial.print(p0);
            Serial.println(" mb/hPa");
          #endif

          temppresPtr->temp = iBMPtemp;
          temppresPtr->abspressure = P;
          temppresPtr->pressure = p0;
        }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");
  }
    
