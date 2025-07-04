#ifndef PCA9548A_H
#define PCA9548A_H

#include "hardware/i2c.h"
#include <cstdint>

class PCA9548A {
public:
    /// Create a new PCA9548A I2C switch object
    /// @param i2c I2C instance (e.g., i2c0 or i2c1)
    /// @param address 7-bit I2C address of the chip (default is 0x70)
    PCA9548A(i2c_inst_t* i2c, uint8_t address = 0x70)
        : _i2c(i2c), _address(address) {}

    /// Selects one or more channels by bitmask
    /// @param channel_mask Bitmask of enabled channels (0x01 = channel 0, 0xFF = all)
    /// @return true if successful
    bool select_channels(uint8_t channel_mask);

    /// Enables a single channel (0–7)
    /// @return true if successful
    bool enable_channel(uint8_t channel);

    /// Disables all channels
    bool disable_all();

private:
    i2c_inst_t* _i2c;
    uint8_t _address;
};

#endif