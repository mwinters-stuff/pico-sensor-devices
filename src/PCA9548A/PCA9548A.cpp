// this is written by me and copilot.
// directly writes and reads to i2c functions.

#include "sensor/PCA9548A.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"

    /// Selects one or more channels by bitmask
    /// @param channel_mask Bitmask of enabled channels (0x01 = channel 0, 0xFF = all)
    /// @return true if successful
    bool PCA9548A::select_channels(uint8_t channel_mask) {
        uint8_t data = channel_mask;
        int result = i2c_write_blocking(_i2c, _address, &data, 1, false);
        return result == 1;
    }

    /// Enables a single channel (0–7)
    /// @return true if successful
    bool PCA9548A::enable_channel(uint8_t channel) {
        if (channel > 7) return false;
        return select_channels(1 << channel);
    }

    /// Disables all channels
    bool PCA9548A::disable_all() {
        return select_channels(0x00);
    }
