/*
   ReadSpektrum.ino : demo sketch for SpektrumDSM library

   Copyright (C) Simon D. Levy 2017

   This file is part of SpektrumDSM.

   SpektrumDSM is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   SpektrumDSM is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   You should have received a copy of the GNU General Public License
   along with SpektrumDSM.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <SpektrumDSM.h>

SpektrumDSM2048 rx;

void setup(void)
{
    rx.begin();

    Serial.begin(115000);
}

void loop(void)
{
    for (int k=0; k<5; ++k) {
        Serial.print("Ch. ");
        Serial.print(k+1);
        Serial.print(": ");
        Serial.print(rx.getChannelValue(k));
        Serial.print("    ");
    }

    Serial.print("Fade count = ");
    Serial.println(rx.getFadeCount());

    delay(10);
}
