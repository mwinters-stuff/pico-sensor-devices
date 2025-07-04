#include <stdio.h>
#include "pico/stdlib.h"
#include "sensor/Wire.h"

Wire wire;

int main(void){
    stdio_init_all();

    wire.begin(i2c_default);

    uint8_t test_addr = 0x50; // Example I2C address (change as needed)
    uint8_t test_data[4] = {0xAA, 0xBB, 0xCC, 0xDD};
    uint8_t read_data[4] = {0};

    // Write test
    wire.beginTransmission(test_addr);
    wire.write(test_data, 4);
    uint8_t result = wire.endTransmission();
    printf("Write result: %u\n", result);

    // Read test
    wire.requestFrom(test_addr, 4);
    wire.readBytes(read_data, 4);

    printf("Read data: %02X %02X %02X %02X\n", read_data[0], read_data[1], read_data[2], read_data[3]);

    while (true) {
        sleep_ms(5000);
    }
}