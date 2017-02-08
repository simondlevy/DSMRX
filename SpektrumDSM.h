#pragma once

#include <stdint.h>

class SpektrumDSM {

    protected:

        void begin(uint8_t cs, uint8_t cm, bool hr, uint8_t nc);

    public:

        bool frameComplete(void);

        uint16_t readRawRC(uint8_t chan);
};

class SpektrumDSM2048 : public SpektrumDSM {

    public:

        void begin(void);
};

class SpektrumDSM1024 : public SpektrumDSM {

    public:

        void begin(void);
};


