#include <stdio.h>

#include "sensor/DHT22.h"

#define GPIO_DHT_PIN 14
DHT dht(GPIO_DHT_PIN, DHT22);

int main(void){
  stdio_init_all();
  dht.begin();
  while(true){
    bool status = dht.read(false);
    if (status) {
      printf("Temperature: %.2f\nHumidity: %.2f\n",
             dht.readTemperature(false, false),
             dht.readHumidity(false));
    } else {
      printf("Temperature/Humidity read failed\n");
    }
    sleep_ms(5000);
  }
}