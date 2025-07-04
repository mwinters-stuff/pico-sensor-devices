#include <stdio.h>
#include "pico/stdlib.h"

#include "sensor/PCF8563.h"

Rtc_Pcf8563 rtc;

int main(void){
  stdio_init_all();

  i2c_init(i2c_default, 100 * 1000);
  gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
  gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
  gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

  rtc.begin(i2c_default);
  rtc.initClock();
  rtc.clearStatus();

  rtc.setDateTime(1, 1, 1, false, 2021, 1, 1, 1);
  rtc.setTimer(1, TMR_1MIN, true);

  while(true){
    rtc.getDateTime();
    
    while (!rtc.timerActive()) {
      printf("RTC Timer not active - back to sleep\n");
      uart_default_tx_wait_blocking();
      sleep_ms(5000);
      rtc.getDateTime();
    }
    
    printf("\n\Triggered At: %s %s UTC\n", rtc.formatDate(RTCC_DATE_WORLD), rtc.formatTime());
    uart_default_tx_wait_blocking();
    rtc.resetTimer();
  }
}