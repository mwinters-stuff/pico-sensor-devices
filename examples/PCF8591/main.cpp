#include <stdio.h>
#include "pico/stdlib.h"
#include "sensor/PCF8591.h"

PCF8591 pcf8591;

int main(void){
  stdio_init_all();

  i2c_init(i2c_default, 100 * 1000);
  gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
  gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
  gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

  pcf8591.begin(i2c_default);

  while(true){
    // Read all 4 analog inputs
    uint8_t ain0 = pcf8591.analogRead(0);
    uint8_t ain1 = pcf8591.analogRead(1);
    uint8_t ain2 = pcf8591.analogRead(2);
    uint8_t ain3 = pcf8591.analogRead(3);

    printf("AIN0: %u, AIN1: %u, AIN2: %u, AIN3: %u\n", ain0, ain1, ain2, ain3);

    // Optionally write to DAC output (example: ramp)
    static uint8_t dac_val = 0;
    pcf8591.analogWrite(dac_val++);
    sleep_ms(1000);
  }
}