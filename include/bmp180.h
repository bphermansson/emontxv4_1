#ifndef BMP180_H_INCLUDED
#define BMP180_H_INCLUDED
    typedef struct
    {
        double temp;
        double abspressure;
        double pressure;
    } bmpvalues;
    void bmp180(bmpvalues *temppresPtr);
#endif