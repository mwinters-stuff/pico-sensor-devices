/*
 * AT24Cxx EEPROM example for Raspberry Pi Pico (C SDK)
 * Tested with AT24C256 (32KB EEPROM, 2-byte addressing)
 */

#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <cstring>

// #include "sensor/PCF8575.h"
#include "sensor/AT24Cx.h"

#define I2C_PORT i2c0
#define SDA_PIN 4
#define SCL_PIN 5

// Example struct to store in EEPROM
typedef struct {
    uint32_t id;
    float temperature;
    char name[16];
} sensor_data_t;

int main() {
    stdio_init_all();
    i2c_init(I2C_PORT, 100 * 1000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    AT24Cx atcx;
    atcx.begin(I2C_PORT);


    printf("\nAT24C EEPROM full test starting...\n");

    bool success = true;

    // Seed random generator
    srand(1234);

    // Allocate buffer to store original test data
    uint8_t *test_data = new uint8_t[EEPROM_SIZE_256];
    
    if (!test_data) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    // Write random bytes using page writes
    printf("Writing test data using page writes...\n");
    for (uint16_t addr = 0; addr < EEPROM_SIZE_256; addr += PAGE_SIZE_256) {
        uint8_t page_buf[PAGE_SIZE_256];
        for (int i = 0; i < PAGE_SIZE_256; i++) {
            uint16_t index = addr + i;
            if (index < EEPROM_SIZE_256) {
                uint8_t val = rand() & 0xFF;
                page_buf[i] = val;
                test_data[index] = val;
            }
        }
        atcx.writePage(addr, page_buf, PAGE_SIZE_256);
        if (addr % 512 == 0) printf("Written up to 0x%04X\n", addr);
    }

    sleep_ms(100);

    // Verify data
    printf("Verifying test data...\n");
    for (uint16_t addr = 0; addr < EEPROM_SIZE_256; ++addr) {
        uint8_t read_val = atcx.readByte(addr);
        if (read_val != test_data[addr]) {
            printf("Mismatch at 0x%04X: wrote 0x%02X, read 0x%02X\n", addr, test_data[addr], read_val);
            success = false;
            break;
        }
        if (addr % 512 == 0) printf("Verified up to 0x%04X\n", addr);
    }

    if (success) {
        printf("Full EEPROM test PASSED.\n");
    } else {
        printf("Full EEPROM test FAILED.\n");
    }

    // --- Struct read/write test ---
    printf("Testing struct read/write...\n");
    sensor_data_t write_struct = { .id = 42, .temperature = 23.5f, .name = "PicoSensor" };
    sensor_data_t read_struct = {0};

    atcx.writePage(0x1000, reinterpret_cast<uint8_t*>(&write_struct), sizeof(write_struct));
    sleep_ms(10); // Wait for write to complete
    atcx.readPage(0x1000, reinterpret_cast<uint8_t*>(&read_struct), sizeof(read_struct));


    if (write_struct.id == read_struct.id &&
        write_struct.temperature == read_struct.temperature &&
        strncmp(write_struct.name, read_struct.name, sizeof(write_struct.name)) == 0) {
        printf("Struct read/write test PASSED.\n");
    } else {
        printf("Struct read/write test FAILED.\n");
        printf("Wrote: id=%u temp=%.2f name=%s\n", write_struct.id, write_struct.temperature, write_struct.name);
        printf("Read:  id=%u temp=%.2f name=%s\n", read_struct.id, read_struct.temperature, read_struct.name);
    }

    // --- Single byte write/read test ---
    printf("Testing single byte write/read...\n");
    uint16_t test_addr = 0x2000;
    uint8_t test_value = 0x5A;
    atcx.writeByte(test_addr, test_value);
    sleep_ms(10); // Wait for write to complete
    uint8_t read_value = atcx.readByte(test_addr);

    if (read_value == test_value) {
        printf("Single byte write/read test PASSED. (0x%02X)\n", read_value);
    } else {
        printf("Single byte write/read test FAILED. Wrote 0x%02X, Read 0x%02X\n", test_value, read_value);
    }
    

    delete[] test_data;
    while (1) tight_loop_contents();
    return 0;
}
