#include <stdio.h>
#include "pico/stdlib.h"

#include "sensor/BH1750.h"

BH1750 bh1750;

int main(void){
  stdio_init_all();

  i2c_init(i2c_default, 100 * 1000);
  gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
  gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
  gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

  bh1750.begin(i2c_default);

  while(true){
    bh1750.powerUp();
    uint16_t light = bh1750.readLightLevel();
    bh1750.powerDown();

    printf("Light: %d\n",light);
    sleep_ms(5000);
  }
}