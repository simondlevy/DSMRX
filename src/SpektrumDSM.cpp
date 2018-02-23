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

#define BUFFER_SIZE  16
#define MAX_CHANS    8

// Modified by serial-event handler
static volatile uint8_t  rxBuf[BUFFER_SIZE];
static volatile uint8_t  rxBufPos;
static volatile uint16_t rcValue[MAX_CHANS];
static volatile uint32_t lastInterruptMicros;

// For communicating between serial-event handler and SpektrumDSM object
static uint8_t _rc_chans;
static uint8_t _chan_shift;
static uint8_t _chan_mask;
static uint8_t _val_shift;
static uint8_t _fade_count;
static bool _got_new_frame;

// Serial-event handler
void DSM_SERIAL_EVENT()
{
    // Reset time 
    lastInterruptMicros = micros();

    // check for new frame, i.e. more than 2.5ms passed
    static uint32_t spekTimeLast;
    uint32_t spekTimeNow = micros();
    uint32_t spekInterval = spekTimeNow - spekTimeLast;
    spekTimeLast = spekTimeNow;
    if (spekInterval > 2500) {
        rxBufPos = 0;
    }

    // put the data in buffer
    while ((DSM_SERIAL.available()) && (rxBufPos < BUFFER_SIZE)) {
        rxBuf[rxBufPos++] = DSM_SERIAL.read();
    }

    // parse frame if done
    if (rxBufPos == BUFFER_SIZE) {

        // grab fade count
        _fade_count = rxBuf[0];

        // convert to channel data in [0,1024]
        for (int b = 2; b < BUFFER_SIZE; b += 2) {
            uint8_t bh = rxBuf[b];
            uint8_t bl = rxBuf[b+1];
            uint8_t spekChannel = 0x0F & (bh >> _chan_shift);
            if (spekChannel < _rc_chans) {
                rcValue[spekChannel] = ((((uint16_t)(bh & _chan_mask) << 8) + bl) >> _val_shift);
            }
        }

        // we have a new frame
        _got_new_frame = true;
    }
}

SpektrumDSM::SpektrumDSM(uint8_t rc, uint8_t cs, uint8_t cm, uint8_t vs)
{
    _rc_chans = rc;
    _chan_shift = cs;
    _chan_mask = cm;
    _val_shift = vs;
}

void SpektrumDSM::begin(void)
{
    _got_new_frame = false;
    DSM_SERIAL.begin(115200);
    lastInterruptMicros = micros();
}

bool SpektrumDSM::gotNewFrame(void)
{
    bool retval = _got_new_frame;
    if (_got_new_frame) {
        _got_new_frame = false;
        delay(5);
    }
    return retval;
}

void SpektrumDSM::getChannelValues(uint16_t values[], uint8_t count)
{
    for (uint8_t k=0; k<count; ++k) {
        values[k] = rcValue[k] + 988;
    }
}

void SpektrumDSM::getChannelValuesNormalized(float values[], uint8_t count)
{
    for (uint8_t k=0; k<count; ++k) {
        values[k] = (rcValue[k] - 512) / 512.f;
    }
}

uint8_t SpektrumDSM::getFadeCount(void)
{
    return _fade_count;
}

bool SpektrumDSM::timedOut(uint32_t maxMicros)
{
    
    uint32_t lag = micros()-lastInterruptMicros;
    return  lag > maxMicros;
}

SpektrumDSM1024::SpektrumDSM1024(void) : SpektrumDSM(7, 2, 0x03, 0)
{
}

SpektrumDSM2048::SpektrumDSM2048(void) : SpektrumDSM(8, 3, 0x07, 1)
{
}
