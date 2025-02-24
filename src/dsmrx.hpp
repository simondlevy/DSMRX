/*
   Spektrum DSM receiver class

   Copyright (C) Simon D. Levy 2017

   This file is part of DSMRX.

   DSMRX is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   DSMRX is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   You should have received a copy of the GNU General Public License
   along with DSMRX.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <stdint.h>

class Dsmrx {

    protected:

        static const uint8_t BUFFER_SIZE  = 16;
        static const uint8_t MAX_CHANS    = 8;

        // Modified by serial-event handler
        volatile uint8_t  _rxBuf[BUFFER_SIZE];
        volatile uint8_t  _rxBufPos;
        volatile uint16_t _rcValue[MAX_CHANS];
        volatile uint32_t _lastInterruptMicros;

        uint8_t _rcChans;
        uint8_t _chanShift;
        uint8_t _chanMask;
        uint8_t _valShift;
        uint8_t _fadeCount;

        bool _gotNewFrame;

    protected:

        Dsmrx(uint8_t rcChans, uint8_t chanShift, uint8_t chanMask, uint8_t valShift)
        {
            _rcChans = rcChans;
            _chanShift = chanShift;
            _chanMask = chanMask;
            _valShift = valShift;

            _gotNewFrame = false;
            _lastInterruptMicros = 0;
        }

    public:

        void parse(uint8_t value, uint32_t usec)
        {
            // Reset time 
            _lastInterruptMicros = usec;

            // check for new frame, i.e. more than 2.5ms passed
            static uint32_t spekTimeLast;
            uint32_t spekTimeNow = usec;
            uint32_t spekInterval = spekTimeNow - spekTimeLast;
            spekTimeLast = spekTimeNow;
            if (spekInterval > 2500) {
                _rxBufPos = 0;
            }

            // put the data in buffer
            if (_rxBufPos < BUFFER_SIZE) {
                _rxBuf[_rxBufPos++] = value;
            }

            // parse frame if done
            if (_rxBufPos == BUFFER_SIZE) {

                // grab fade count
                _fadeCount = _rxBuf[0];

                // convert to channel data in [0,1024]
                for (int b = 2; b < BUFFER_SIZE; b += 2) {
                    uint8_t bh = _rxBuf[b];
                    uint8_t bl = _rxBuf[b+1];
                    uint8_t spekChannel = 0x0F & (bh >> _chanShift);
                    if (spekChannel < _rcChans) {
                        _rcValue[spekChannel] =
                            ((((uint16_t)(bh & _chanMask) << 8) + bl) >> _valShift);
                    }
                }

                // we have a new frame
                _gotNewFrame = true;
            }
        }

        bool gotNewFrame(void)
        {
            bool retval = _gotNewFrame;
            if (_gotNewFrame) {
                _gotNewFrame = false;
            }
            return retval;
        }

        /**
         * Returns channel values in [-1,+1] interval
         */
        void getChannelValues(float values[], uint8_t count=8)
        {
            for (uint8_t k=0; k<count; ++k) {
                values[k] = (_rcValue[k] - 512) / 512.f;
            }
        }

        uint8_t getFadeCount(void)
        {
            return _fadeCount;
        }

        bool timedOut(uint32_t usec, uint32_t maxMicros=100000)
        {
            uint32_t lag = usec - _lastInterruptMicros;
            return  lag > maxMicros;
        }

}; // class Dsmrx 

class Dsm1024 : public Dsmrx {

    public:

        Dsm1024(void)
            : Dsmrx(7, 2, 0x03, 0)        
        {
        }
};

class Dsm2048 : public Dsmrx {

    public:

        Dsm2048(void)
            : Dsmrx(8, 3, 0x07, 1)
        {
        }

        // For when constructor won't work
        void init(void)
        {
            _rcChans = 8;
            _chanShift = 3;
            _chanMask = 0x07;
            _valShift = 1;

            _gotNewFrame = false;
            _lastInterruptMicros = 0;
         }
};
