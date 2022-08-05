#ifndef HTU21D_H_INCLUDED
#define HTU21D_H_INCLUDED
    #include <settings.h>
    typedef struct
    {
        float temp;
        float humidity;
    } htuvalues;
    void read_htu21d(htuvalues *htuvalues);
#endif