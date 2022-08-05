// See https://github.com/enjoyneering/BH1750FVI
#ifndef BH1750_READ_H_INCLUDED
#define BH1750_READ_H_INCLUDED
    #include <Wire.h>
    #include <BH1750FVI.h>
    #include <settings.h>
    void bh1750fvi(uint8_t *luxPtr);
#endif