#include "sensor/Wire.h"
// #include <stdio.h>
#include "pico/stdlib.h"


Wire::Wire(){

}

void Wire::begin(i2c_inst_t *i2c){
  this->i2c = i2c;
  usedTxBuffer = 0;
}

void Wire::beginTransmission(uint8_t address){
  _address = address;// << 1;
  usedTxBuffer = 0;
}

uint8_t Wire::endTransmission(bool stopBit){
  if (usedTxBuffer == 0) {
    // we are scanning, return 0 if the addresed device responds with an ACK
      uint8_t buf[1];

      int ret = i2c_read_blocking(i2c, _address, buf, 1, !stopBit);
      return ret <= PICO_ERROR_GENERIC ? 0xff : ret;
    }


    if (i2c_write_blocking_until(i2c, _address, txBuffer, usedTxBuffer, !stopBit, make_timeout_time_ms(1000)) == PICO_ERROR_GENERIC){
      // printf("endTransmission Timed Out\n");
      return 0xff;
    } 
    return usedTxBuffer;

}

uint8_t Wire::endTransmission(void) {
	return endTransmission(true);
}

size_t Wire::write(uint8_t data){
  if (usedTxBuffer >= 256){
    return 0;
  } 
	txBuffer[usedTxBuffer++] = data;
	return 1;
}


size_t Wire::requestFrom(uint8_t address, size_t len, bool stopBit) {
	uint8_t buf[256];
	int ret = i2c_read_blocking(i2c, address , buf, len, !stopBit); //<< 1
	if (ret == PICO_ERROR_GENERIC) {
		return 0;
	}
	for (size_t i=0; i<len; i++) {
		rxBuffer.store_char(buf[i]);
	}
	return len;
}

size_t Wire::requestFrom(uint8_t address, size_t len) {
	return requestFrom(address, len, true);
}


int Wire::read() {
	if (rxBuffer.available()) {
		return rxBuffer.read_char();
	}
	return 0;
}

int Wire::available() {
	return rxBuffer.available();
}
