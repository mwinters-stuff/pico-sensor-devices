#include <stdio.h>
#include "pico/stdlib.h"
#include "sensor/VEML6070.h"

VEML6070 veml6070;

int main(void){
  stdio_init_all();

  i2c_init(i2c_default, 100 * 1000);
  gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
  gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
  gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

  veml6070.begin(i2c_default, VEML6070_4_T);

  while(true){
    veml6070.sleep(false);
    uint16_t uv = veml6070.readUV();
    veml6070.sleep(true);

    printf("UV: %u\n", uv);
    sleep_ms(5000);
  }
}