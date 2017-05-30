#include <SpektrumDSM.h>

void setup(void)
{
    initRX();

    Serial.begin(115000);
}

void loop(void)
{
    extern uint16_t rcValue[];

    uint8_t chanmap[5] = {1, 2, 3, 0, 5};

    for (int k=0; k<5; ++k) {
        Serial.print(rcValue[chanmap[k]]);
        Serial.print(" ");
    }
    Serial.println();

    delay(10);
}
