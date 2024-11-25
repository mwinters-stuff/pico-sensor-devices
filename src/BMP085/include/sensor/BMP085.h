#ifndef _BMP085_H_
#define _BMP085_H_
/*!
 * @file Adafruit_BMP085_U.cpp
 *
 * @mainpage Adafruit BMP085 Pressure Sensor
 *
 * @section intro_sec Introduction
 *
 * This is a library for the BMP085 pressure sensor
 *
 * Designed specifically to work with the Adafruit BMP085 or BMP180 Breakout
 * ----> http://www.adafruit.com/products/391
 * ----> http://www.adafruit.com/products/1603
 *
 * These displays use I2C to communicate, 2 pins are required to interface.
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit andopen-source hardware by purchasing products
 * from Adafruit!
 *
 * @section author Author
 *
 * Written by Kevin Townsend for Adafruit Industries.
 *
 * @section license License
 * BSD license, all text above must be included in any redistribution
 */

#include "sensor/Wire.h"

#define BMP085_ADDR (0x77)

/*!
 * @brief BMP085 I2C registers
 */
enum {
  BMP085_REGISTER_CAL_AC1         = 0xAA, //!< R   Calibration data (16 bits)
  BMP085_REGISTER_CAL_AC2         = 0xAC, //!< R   Calibration data (16 bits)
  BMP085_REGISTER_CAL_AC3         = 0xAE, //!< R   Calibration data (16 bits)
  BMP085_REGISTER_CAL_AC4         = 0xB0, //!< R   Calibration data (16 bits)
  BMP085_REGISTER_CAL_AC5         = 0xB2, //!< R   Calibration data (16 bits)
  BMP085_REGISTER_CAL_AC6         = 0xB4, //!< R   Calibration data (16 bits)
  BMP085_REGISTER_CAL_B1          = 0xB6, //!< R   Calibration data (16 bits)
  BMP085_REGISTER_CAL_B2          = 0xB8, //!< R   Calibration data (16 bits)
  BMP085_REGISTER_CAL_MB          = 0xBA, //!< R   Calibration data (16 bits)
  BMP085_REGISTER_CAL_MC          = 0xBC, //!< R   Calibration data (16 bits)
  BMP085_REGISTER_CAL_MD          = 0xBE, //!< R   Calibration data (16 bits)
  BMP085_REGISTER_CHIPID          = 0xD0, //!< Register that contains the chip ID
  BMP085_REGISTER_VERSION         = 0xD1, //!< Register that contains the chip version
  BMP085_REGISTER_SOFTRESET       = 0xE0, //!< Register for doing a soft reset
  BMP085_REGISTER_CONTROL         = 0xF4, //!< Control register
  BMP085_REGISTER_TEMPDATA        = 0xF6, //!< Temperature data register
  BMP085_REGISTER_PRESSUREDATA    = 0xF6, //!< Pressure data register
  BMP085_REGISTER_READTEMPCMD     = 0x2E, //!< Read temperature control register value
  BMP085_REGISTER_READPRESSURECMD = 0x34  //!< Read pressure control register value
};

/*!
 * @brief BMP085 mode settings
 */
typedef enum {
  BMP085_MODE_ULTRALOWPOWER = 0,
  BMP085_MODE_STANDARD = 1,
  BMP085_MODE_HIGHRES = 2,
  BMP085_MODE_ULTRAHIGHRES = 3
} bmp085_mode_t;
/*=========================================================================*/

/*!
 * @brief Calibration data
 */
typedef struct {
  int16_t ac1;  //!< R calibration coefficient (16-bits)
  int16_t ac2;  //!< R calibration coefficient (16-bits)
  int16_t ac3;  //!< R calibration coefficient (16-bits)
  uint16_t ac4; //!< R calibration coefficient (16-bits)
  uint16_t ac5; //!< R calibration coefficient (16-bits)
  uint16_t ac6; //!< R calibration coefficient (16-bits)
  int16_t b1;   //!< R calibration coefficient (16-bits)
  int16_t b2;   //!< R calibration coefficient (16-bits)
  int16_t mb;   //!< R calibration coefficient (16-bits)
  int16_t mc;   //!< R calibration coefficient (16-bits)
  int16_t md;   //!< R calibration coefficient (16-bits)
} bmp085_calib_data;


/**************************************************************************/
/*!
    @brief  Class that stores state and functions for interacting with BMP085
*/
/**************************************************************************/
class BMP085 {
public:
  BMP085() = default;

  bool begin(i2c_inst_t* i2c, bmp085_mode_t mode = BMP085_MODE_ULTRAHIGHRES);
  float readTemperature();
  float readPressure();

private:
  void writeCommand(uint8_t reg, uint8_t value);
  void readCoefficients();
  int32_t readRawTemperature();
  int32_t readRawPressure();
  int32_t computeB5(int32_t ut);
  uint8_t read8(uint8_t reg);
  uint16_t read16(uint8_t reg);
  int16_t readS16(uint8_t reg);
  Wire _i2c;
  uint8_t _bmp085Mode;
  bmp085_calib_data _bmp085_coeffs;
};

#endif