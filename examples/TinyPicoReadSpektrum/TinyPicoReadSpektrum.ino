/*
   Demo sketch for dsmrx library on TinyPico

   Displays channel values in interval [1000, 2000]

   Copyright (C) Simon D. Levy 2019

   This file is part of dsmrx.

   dsmrx is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   dsmrx is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   You should have received a copy of the GNU General Public License
   along with dsmrx.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <dsmrx.h>

static const uint8_t SERIAL1_RX = 4;
static const uint8_t SERIAL1_TX = 14; // unused

static const uint8_t CHANNELS = 8;

Dsm2048 rx;

static void coreTask(void * params)
{
    while (true) {
      
        if (Serial1.available()) {
           rx.handleSerialEvent(Serial1.read(), micros()); 
        }

        delay(1);
    } 
}

void setup(void)
{
    Serial.begin(115000);

    Serial1.begin(115000, SERIAL_8N1, SERIAL1_RX, SERIAL1_TX);

    TaskHandle_t task;
    xTaskCreatePinnedToCore(coreTask, "Task", 10000, NULL, 1, &task, 0); 
}

void loop(void)
{
    if (rx.timedOut(micros())) {
        Serial.println("*** TIMED OUT ***");
    }

    else if (rx.gotNewFrame()) {

        uint16_t values[CHANNELS];

        rx.getChannelValues(values, CHANNELS);

        for (int k=0; k<CHANNELS; ++k) {
            Serial.print("Ch. ");
            Serial.print(k+1);
            Serial.print(": ");
            Serial.print(values[k]);
            Serial.print("    ");
        }

        Serial.print("Fade count = ");
        Serial.println(rx.getFadeCount());
    }

    // Need a little loop delay
    delay(1);
}
