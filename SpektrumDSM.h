/*
   SpektrumDSM.h : header for interrupt-based Spektrum DSM receiver for Arduino

   This file is part of SpektrumDSM.

   SpektrumDSM is free software: you can redistribute it and/or modify
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

#include <stdint.h>

class SpektrumDSM {

    private:

        uint8_t m_chan_shift;
        uint8_t m_chan_mask;
        uint8_t m_right_shift;

    protected:

        SpektrumDSM(uint8_t chan_shift, uint8_t chan_mask, uint8_t right_shift) :
            m_chan_shift(chan_shift), m_chan_mask(chan_mask), m_right_shift(right_shift) { }


    public:

        void begin();

        uint16_t getChannelValue(uint8_t chan);
};

class SpektrumDSM1024 : public SpektrumDSM {

    public:

        SpektrumDSM1024(void) : SpektrumDSM(2, 0x03, 0) { }
};

class SpektrumDSM2048 : public SpektrumDSM {

    public:

        SpektrumDSM2048(void) : SpektrumDSM(3, 0x07, 1) { }
};
