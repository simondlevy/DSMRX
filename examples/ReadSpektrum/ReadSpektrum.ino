#include "SpektrumDSM.h"

void setup() {
    initRX();
}

void loop() {
    for (int k=0; k<8; ++k) {
        Serial.print(getRcValue(k));
        Serial.print("  ");
    }
    Serial.println();
}
