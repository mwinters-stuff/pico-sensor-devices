#ifndef AT24Cx_H
#define AT24Cx_H  

#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define AT24C_ADDR 0x50  // Base I2C address

#define PAGE_SIZE_256 64      // AT24C256 page size
#define EEPROM_SIZE_256 32768 // AT24C256 32KB


class AT24Cx
{
public:
  explicit AT24Cx(const uint16_t eeprom_size = EEPROM_SIZE_256, const uint8_t page_size = PAGE_SIZE_256);

  void begin(i2c_inst_t* i2c, const uint8_t deviceAddress = AT24C_ADDR);

  void writePage(uint16_t mem_address, uint8_t *data, uint16_t length);
  void readPage(uint16_t mem_address, uint8_t *data, uint16_t length);

  uint8_t readByte(uint16_t mem_address);
  void writeByte(uint16_t mem_address, uint8_t data);
private:
  uint8_t  _address;
  uint8_t  _pageSize;
  uint16_t _eepromSize;
  i2c_inst_t* _i2c;


};


#endif