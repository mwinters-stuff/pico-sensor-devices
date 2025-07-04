#include <stdio.h>
#include "pico/stdlib.h"

#include "sensor/BMP085.h"

BMP085 bmp085;

int main(void){
  stdio_init_all();

  i2c_init(i2c_default, 100 * 1000);
  gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
  gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
  gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

  bmp085.begin(i2c_default);

  while(true){
    float temperature = bmp085.readTemperature();
    float pressure = bmp085.readPressure();

    printf("Temperature: %f\nPressure: %f\n",temperature,pressure);
    sleep_ms(5000);
  }
}