#include "SpektrumDSM.h"

#include <Arduino.h>

#define SPEK_FRAME_SIZE 16       

// 1024 mode
//#define SPEK_CHAN_SHIFT  2       
//#define SPEK_CHAN_MASK   0x03    
//#define SPEK_RIGHT_SHIFT 0

// 2048 mode
#define SPEK_CHAN_SHIFT  3       
#define SPEK_CHAN_MASK   0x07    
#define SPEK_RIGHT_SHIFT 1

volatile uint16_t rcValue[8];

#define RX_BUFFER_SIZE (16)
static volatile uint8_t rxBufPos;
static uint8_t rxBuf[RX_BUFFER_SIZE];

void initRX() {
  // serial port for spektrum sat rx
  Serial1.begin(115200);  
}

uint16_t getRcValue(uint8_t chan)
{
    return rcValue[chan];
}

// parse raw serial data into channels
static void parseRXData() {
  // convert to channel data in the 1000-2000 range
  for (int b = 2; b < SPEK_FRAME_SIZE; b += 2)
  {
    uint8_t bh = rxBuf[b];
    uint8_t bl = rxBuf[b+1];
    uint8_t spekChannel = 0x0F & (bh >> SPEK_CHAN_SHIFT);
    if (spekChannel < 8)  
        rcValue[spekChannel] = 988 + ((((uint16_t)(bh & SPEK_CHAN_MASK) << 8) + bl) >> SPEK_RIGHT_SHIFT);
  }
}


// check for new frame, i.e. more than 2.5ms passed
uint32_t spekTimeLast = 0;
void checkForNewFrame() {
  uint32_t spekTimeNow = micros();
  uint32_t spekInterval = spekTimeNow - spekTimeLast;
  spekTimeLast = spekTimeNow;
  if (spekInterval > 2500) {
    rxBufPos = 0;
  }
}

// RX interrupt
void serialEvent1() {

  checkForNewFrame();
  
  // put the data in buffer
  while ((Serial1.available()) && (rxBufPos < RX_BUFFER_SIZE))
    rxBuf[rxBufPos++] = (char)Serial1.read();

  // parse frame if done
  if (rxBufPos == SPEK_FRAME_SIZE)
    parseRXData();
}
