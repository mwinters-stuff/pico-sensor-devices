# pico-sensor-devices
Arduino Code for various Sensors modified to work in the pico-c-sdk

## Sensors

* DHT22, Temperature and Humidity
* BH1750, Light (Lux)
* INA226, Voltage and Current
* PCF8563, RTC
* VEML6070, UV (Ultra Violet)
* BMP085/BMP180, Temperature and Atmospheric Pressure
* PCF8591, 8-bit A/D and D/A converter
* PCF8575, 16 port I/O Expander

Feel free to issue PR's with additional devices.

## Wire
All the sensors apart from the DHT22 are I2C sensors, these to be compatible with the arduino sensor code, 
the wire class provides a compatible interface and is used by the above sensors.

Each sensor class holds its own instance of wire, a "begin" method has been added or modified to pass in the
pico i2c device.

## Using

To use the library, follow these steps, it will be similar to that you would have used to use the "pico-extras" library.

1. Download the `pico_sensors_import.cmake` file to the root of your project.
   > wget https://github.com/mwinters-stuff/pico-sensor-devices/blob/master/pico_sensors_import.cmake
2. Add the following lines to your `CMakeLists.txt` before the `pico_sdk_init()` call
   ```
      set(PICO_SENSORS_FETCH_FROM_GIT 1)
      include(pico_sensors_import.cmake)
    ```
3. Target the libraries you are using, each one is prefixed `sensor_`:
   ```
    target_link_libraries(my-project-name
      sensor_DHT22
      sensor_INA226
      sensor_PCF8563
      sensor_VEML6070
      sensor_BH1750
      sensor_bmp085
      sensor_PCF8591
      sensor_PCF8575
    )
   ```
4. In your code, include the header you need, if you didnt target the sensor then the header wont be found.
   ```CPP
   #include "sensor/BH1750.h"
   #include "sensor/INA226.h"
   #include "sensor/PCF8563.h"
   #include "sensor/VEML6070.h"
   #include "sensor/DHT22.h"
   #include "sensor/BMP085.h"
   #include "sensor/PCF8591.h"
   #include "sensor/PCF8575.h"
   ```
5. Follow the instructions for each sensor.

*Warning the code bellow has been extracted from a project and actually might not compile or work, use as example only*
### DHT22
The following code will setup the DHT22 and read from it
```CPP
#include <sstream>
#include <stdio.h>

#include "sensor/DHT22.h"

#define GPIO_DHT_PIN 14
DHT dht(GPIO_DHT_PIN, DHT22);

int main(void){
  stdio_init_all();
  dht.begin();
  while(true){
    bool status = dht.read(false);
    std::stringbuffer sbuff;
    if (status) {
      
      sbuff << "Temperature: " << dht.readTemperature(false, false) << "\n"
            << "Humidity: " << dht.readHumidity(false) << "\n";
    } else {
      sbuff << "Temperature/Humitity read failed \n";
    }
    printf(sbuff.str().c_str());
    sleep(5000);
  }
}
```

### BH1750
The following code will setup the BH1750 and read from it
```CPP
#include <sstream>
#include <stdio.h>

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
    std::stringbuffer sbuff;
  
    bh1750.powerUp();
    uint16_t light = bh1750.readLightLevel();
    bh1750.powerDown();
    sbuff << "Light: " << light << "\n";

    printf(sbuff.str().c_str());
    sleep(5000);
  }
}
```

## VEML6070
The following code will setup the VEML6070 and read from it
```CPP
#include <sstream>
#include <stdio.h>

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
    std::stringbuffer sbuff;
  
    veml6070.sleep(false);
    uint16_t uv = veml6070.readUV();
    sbuff << "UV: " << uv << "\n";
    veml6070.sleep(true);

    printf(sbuff.str().c_str());
    sleep(5000);
  }
}
```
## INA226
The following code will setup the INA226 and read from it
```CPP
#include <sstream>
#include <stdio.h>

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
    std::stringbuffer sbuff;
  
    if (ina_battery.triggerAndWait(INA226_MODE_SHUNT_BUS_TRIG)) {
      sbuff << "Battery Voltage: " << ina_battery.readBusVoltage() << "\n"
            << "Battery Current: " << (ina_battery.readShuntCurrent() * 1000) << "\n";
    } else {
      sbuff << "Battery Voltage/Current Conversion Failed: Timeout\n";
    }
    ina_battery.powerDown();

    printf(sbuff.str().c_str());
    sleep(5000);
  }
}
```

## PCF8563
The following code will setup the PCF8563 RTC and read from it
```CPP
#include <sstream>
#include <stdio.h>

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
      sleep(5000);
      rtc.getDateTime();
    }
    
    printf("\n\Triggered At: %s %s UTC\n", rtc.formatDate(RTCC_DATE_WORLD), rtc.formatTime());
    uart_default_tx_wait_blocking();
    rtc.resetTimer();
  }
}
```
## BMP085/BMP180
The following code will setup the BMP085 or BMP180 and read from it
```CPP
#include <sstream>
#include <stdio.h>

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
    std::stringbuffer sbuff;
  
    float temperature = bmp085.readTemparature();
    float pressure = bmp085.readPressure();
    sbuff << "Temperature: " << temperature << "\n"
          << "Pressure: " << pressure << "\n";

    printf(sbuff.str().c_str());
    sleep(5000);
  }
}
```

## PCF8575
The following code writes to the PCF8575
```CPP
#include "hardware/i2c.h"
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

```