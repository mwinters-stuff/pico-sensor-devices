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
      sensor_ADS1X15
      sensor_AT24Cx
      sensor_BH1750
      sensor_BMP085
      sensor_DHT22
      sensor_INA226
      sensor_PCF8563
      sensor_PCF8575
      sensor_PCF8591
      sensor_VEML6070
    )
   ```
4. In your code, include the header you need, if you didnt target the sensor then the header wont be found.
   ```CPP
   #include "sensor/ADS1X15.h"
   #include "sensor/AT24Cx.h"
   #include "sensor/BH1750.h"
   #include "sensor/BMP085.h"
   #include "sensor/DHT22.h"
   #include "sensor/INA226.h"
   #include "sensor/PCF8563.h"
   #include "sensor/PCF8575.h"
   #include "sensor/PCF8591.h"
   #include "sensor/VEML6070.h"
   ```
5. Look in the examples directory, cmake will give you the targets to run and code.

