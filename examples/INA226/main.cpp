#include <stdio.h>
#include "pico/stdlib.h"

#include "sensor/INA226.h"

INA226 ina226;

int main(void){
  stdio_init_all();

  i2c_init(i2c_default, 100 * 1000);
  gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
  gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
  gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

  ina226.begin(i2c_default, INA226_ADDRESS);
  ina226.calibrate();
  ina226.configure(INA226_AVERAGES_16, INA226_BUS_CONV_TIME_1100US, INA226_SHUNT_CONV_TIME_1100US, INA226_MODE_SHUNT_BUS_TRIG);

  while(true){
    if (ina226.triggerAndWait(INA226_MODE_SHUNT_BUS_TRIG)) {
      printf("Battery Voltage: %.3f V\n", ina226.readBusVoltage());
      printf("Battery Current: %.3f mA\n", ina226.readShuntCurrent() * 1000);
    } else {
      printf("Battery Voltage/Current Conversion Failed: Timeout\n");
    }
    ina226.powerDown();

    sleep_ms(5000);
  }
}