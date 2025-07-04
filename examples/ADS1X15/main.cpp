/*
 * AT24Cxx EEPROM example for Raspberry Pi Pico (C SDK)
 * Tested with AT24C256 (32KB EEPROM, 2-byte addressing)
 */

#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <cstring>
#include "sensor/ADS1X15.h"

#define I2C_PORT i2c0
#define SDA_PIN 4
#define SCL_PIN 5

int main() {
    stdio_init_all();
    i2c_init(I2C_PORT, 100 * 1000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    printf("ADS1X15 ADC Test\n");

    ADS1115 ads;
    ads.begin(I2C_PORT);

    if (!ads.isConnected()) {
        printf("ADS1X15 not detected on I2C bus!\n");
        return 1;
    }

    printf("ADS1X15 detected. Reading values...\n");

    while (true) {
        int16_t value0 = ads.readADC(0);
        int16_t value1 = ads.readADC(1);
        int16_t value2 = ads.readADC(2);
        int16_t value3 = ads.readADC(3);

        printf("AIN0: %d, AIN1: %d, AIN2: %d, AIN3: %d\n", value0, value1, value2, value3);

        sleep_ms(1000);
    }
}
