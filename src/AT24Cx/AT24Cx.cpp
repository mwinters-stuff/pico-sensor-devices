// this is written by me and copilot.
// directly writes and reads to i2c functions.

#include "sensor/AT24Cx.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <cstring>

AT24Cx::AT24Cx(const uint16_t eeprom_size, const uint8_t page_size):
  _eepromSize(eeprom_size),
  _pageSize(page_size)
{
}

void AT24Cx::begin(i2c_inst_t *i2c, const uint8_t deviceAddress)
{
  _address = deviceAddress;
  _i2c = i2c;
}


void AT24Cx::writePage(uint16_t mem_address, uint8_t *data, uint16_t length)
{

    if (length > _pageSize) return; // Prevent overflow

    uint8_t buf[2 + length];
    buf[0] = (uint8_t)(mem_address >> 8);
    buf[1] = (uint8_t)(mem_address & 0xFF);
    memcpy(&buf[2], data, length);

    i2c_write_blocking(_i2c, AT24C_ADDR, buf, length + 2, false);

    sleep_ms(5); // Allow EEPROM to finish write

}

void AT24Cx::readPage(uint16_t mem_address, uint8_t *data, uint16_t length)
{
    if (length > _pageSize) return; // Prevent overflow

    uint8_t addr[2];
    addr[0] = (uint8_t)(mem_address >> 8);
    addr[1] = (uint8_t)(mem_address & 0xFF);

    i2c_write_blocking(_i2c, AT24C_ADDR, addr, 2, true);
    i2c_read_blocking(_i2c, AT24C_ADDR, data, length, false);
}

uint8_t AT24Cx::readByte(uint16_t mem_address)
{
      uint8_t addr[2] = {
        (uint8_t)(mem_address >> 8),
        (uint8_t)(mem_address & 0xFF)
    };
    uint8_t data = 0;
    i2c_write_blocking(_i2c, AT24C_ADDR, addr, 2, true);
    i2c_read_blocking(_i2c, AT24C_ADDR, &data, 1, false);

    return data;
}

void AT24Cx::writeByte(uint16_t mem_address, uint8_t data)
{
    uint8_t buf[3];
    buf[0] = (uint8_t)(mem_address >> 8);
    buf[1] = (uint8_t)(mem_address & 0xFF);
    buf[2] = data;

    i2c_write_blocking(_i2c, AT24C_ADDR, buf, 3, false);

    sleep_ms(5); // Allow EEPROM to finish write
}