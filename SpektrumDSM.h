#pragma once

#include <stdint.h>

typedef enum {
    SERIALRX_SPEKTRUM1024,
    SERIALRX_SPEKTRUM2048
} serialrx_t;

class SpektrumDSM {

    public:

        void begin(serialrx_t serialrx_type);

        bool frameComplete(void);

        uint16_t readRawRC(uint8_t chan);
};


