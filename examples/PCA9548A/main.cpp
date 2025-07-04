#include "sensor/PCA9548A.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"

int main() {
    stdio_init_all();

    // Initialize I2C0 on GPIO 4 (SDA) and GPIO 5 (SCL)
    i2c_init(i2c0, 100 * 1000);
    gpio_set_function(4, GPIO_FUNC_I2C);
    gpio_set_function(5, GPIO_FUNC_I2C);
    gpio_pull_up(4);
    gpio_pull_up(5);

    // Create PCA9548A object
    PCA9548A mux(i2c0);

    // Enable channel 2
    mux.enable_channel(2);

    sleep_ms(1000);

    // Enable channels 0 and 3 together
    mux.select_channels((1 << 0) | (1 << 3));

    sleep_ms(1000);

    // Disable all
    mux.disable_all();

    while (true) {
        tight_loop_contents();
    }
}