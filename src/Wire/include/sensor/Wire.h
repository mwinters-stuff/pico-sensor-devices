#ifndef _WIRE_H_
#define _WIRE_H_

#include <inttypes.h>
#include "hardware/i2c.h"
#include "RingBuffer.h"


class Wire {
  public:
    Wire();
    void begin(i2c_inst_t* i2c);
    size_t write(uint8_t byte);
    void beginTransmission(uint8_t address);
    uint8_t endTransmission(bool stopBit);
    uint8_t endTransmission();
    size_t requestFrom(uint8_t address, size_t len, bool stopBit);
    size_t requestFrom(uint8_t address, size_t len);
    int available();
    int read();
    size_t write(const uint8_t* data, size_t size) ;
    size_t readBytes(uint8_t* buffer, size_t length);

  private:
     i2c_inst_t* i2c;
    uint8_t _address;
    uint8_t txBuffer[256];
    uint32_t usedTxBuffer;
    RingBufferN<256> rxBuffer;

};

#endif