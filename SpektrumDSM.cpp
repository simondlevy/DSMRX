/*
   SpektrumDSM.cpp : implementation of interrupt-based Spektrum DSM receiver for Arduino

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

#include <Arduino.h>
#include "SpektrumDSM.h"

#define MIDRC 1500
#define RX_BUFFER_SIZE (16)
#define RC_CHANS (12)
#define SPEK_FRAME_SIZE 16 

//#define SPEK_CHAN_SHIFT  2       // Assumes 10 bit frames, that is 1024 mode.
//#define SPEK_CHAN_MASK   0x03    // Assumes 10 bit frames, that is 1024 mode.

#define SPEK_CHAN_SHIFT  3       // Assumes 11 bit frames, that is 2048 mode.
#define SPEK_CHAN_MASK   0x07    // Assumes 11 bit frames, that is 2048 mode.

volatile uint8_t rxBuf[RX_BUFFER_SIZE];
volatile uint8_t rxBufPos;
volatile uint16_t rcValue[RC_CHANS] = {1000, MIDRC, MIDRC, MIDRC, MIDRC, MIDRC, MIDRC, MIDRC, MIDRC, MIDRC, MIDRC, MIDRC}; // throttle off


void initRX() 
{
    Serial1.begin(115200);  
}

// check for new frame, i.e. more than 2.5ms passed
static uint32_t spekTimeLast;
static void checkForNewFrame() 
{
    uint32_t spekTimeNow = micros();
    uint32_t spekInterval = spekTimeNow - spekTimeLast;
    spekTimeLast = spekTimeNow;
    if (spekInterval > 2500) {
        rxBufPos = 0;
    }
}

// parse raw serial data into channels
static void parseRXData() 
{
    // convert to channel data in the 1000-2000 range
    for (int b = 2; b < SPEK_FRAME_SIZE; b += 2) {
        uint8_t bh = rxBuf[b];
        uint8_t bl = rxBuf[b+1];
        uint8_t spekChannel = 0x0F & (bh >> SPEK_CHAN_SHIFT);
        if (spekChannel < RC_CHANS) {
            rcValue[spekChannel] = ((((uint16_t)(bh & SPEK_CHAN_MASK) << 8) + bl) >> 1) + 988;
        }
    }
}

// RX interrupt
void serialEvent1() 
{
    checkForNewFrame();

    // put the data in buffer
    while ((Serial1.available()) && (rxBufPos < RX_BUFFER_SIZE))
        rxBuf[rxBufPos++] = (char)Serial1.read();

    // parse frame if done
    if (rxBufPos == SPEK_FRAME_SIZE)
        parseRXData();
}
