/*
   ReadSpektrum.ino : example of how to read from a Spektrum DSM receiver using Arduino

   This file is part of SpektrumDSM.

   Hackflight is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   Hackflight is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   You should have received a copy of the GNU General Public License
   along with Hackflight.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "SpektrumDSM.h"

SpektrumDSM rx;

void setup() {
    rx.begin();
}

void loop() {
    for (int k=0; k<8; ++k) {
        Serial.print(rx.getChannelValue(k));
        Serial.print("  ");
    }
    Serial.println();
}
