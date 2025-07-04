#include "pico/stdlib.h"
#include "sensor/PCF8575.h"
#include <stdio.h>

int main()
{
  stdio_init_all();

  // I2C Initialisation. Using it at 400Khz.
  i2c_init(i2c_default, 100 * 1000);
  gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
  gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
  gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

  PCF8575 PCF(0x20);
  PCF.begin(i2c_default);
  // For more examples of I2C use see https://github.com/raspberrypi/pico-examples/tree/master/i2c

  while (true)
  {
    PCF.write(0, 1);
    for (int i = 0; i < 15; i++)
    {
      PCF.rotateLeft();
      sleep_ms(100);
    }

    for (int i = 0; i < 15; i++)
    {
      PCF.rotateRight();
      sleep_ms(100);
    }

    for (int i = 0; i < 15; i++)
    {
      PCF.rotateLeft(3);
      sleep_ms(100);
    }

    for (int i = 0; i < 15; i++)
    {
      PCF.rotateRight(2);
      sleep_ms(100);
    }

    for (uint16_t i = 0; i < 65535; i += 253)
    {
      PCF.toggleMask(i);
      sleep_ms(100);
    }

    //  0010 0111  -> 0x27
    //  1110 0100
    PCF.write16(0x2755);
    for (int i = 0; i < 255; i++)
    {
      PCF.reverse();
      sleep_ms(100);
    }
  }
}