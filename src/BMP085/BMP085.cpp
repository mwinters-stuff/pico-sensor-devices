/*!
 * @file BMP085.cpp
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
 * These devices use I2C to communicate, 2 pins are required to interface.
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

#include "sensor/BMP085.h"

#define _sleep_us busy_wait_us
#define _sleep_ms busy_wait_ms

// #define _sleep_us sleep_us
// #define _sleep_ms sleep_ms

#define BMP085_USE_DATASHEET_VALS                                              \
  (0) //!< Set to 1 for sanity check, when true, will use values from datasheet

/**************************************************************************/
/*!
    @brief  setup and initialize communication with the hardware
    @param itime the integration time to use for the data
    @param twoWire Optional pointer to the desired TwoWire I2C object. Defaults
   to &Wire
*/
/**************************************************************************/
bool BMP085::begin(i2c_inst_t* i2c, bmp085_mode_t mode) {
  _i2c.begin(i2c);

  /* Mode boundary check */
  if ((mode > BMP085_MODE_ULTRAHIGHRES) || (mode < 0)) {
    mode = BMP085_MODE_ULTRAHIGHRES;
  }

  /* Make sure we have the right device */
  auto id = read8(BMP085_REGISTER_CHIPID);
  if (id != 0x55) {
    return false;
  }

  /* Set the mode indicator */
  _bmp085Mode = mode;

  /* Coefficients need to be read once */
  readCoefficients();

  return true;
}

/**************************************************************************/
/*!
    @brief  Reads the temperatures in degrees Celsius
*/
/**************************************************************************/
float BMP085::readTemperature() {
  int32_t UT, B5; // following ds convention
  float t;

  UT = readRawTemperature();

#if BMP085_USE_DATASHEET_VALS
  // use datasheet numbers!
  _bmp085_coeffs.ac6 = 23153;
  _bmp085_coeffs.ac5 = 32757;
  _bmp085_coeffs.mc = -8711;
  _bmp085_coeffs.md = 2868;
  UT = 27898;

#endif

  B5 = computeB5(UT);
  t = (B5 + 8) >> 4;
  t /= 10;

  return t;
}

/**************************************************************************/
/*!
    @brief  Gets the compensated pressure level in kPa
*/
/**************************************************************************/
float BMP085::readPressure() {
  int32_t x1, x2, b5, b6, x3, b3, p;
  uint32_t b4, b7;

  /* Get the raw pressure and temperature values */
  auto ut = readRawTemperature();
  auto up = readRawPressure();

  /* Temperature compensation */
  b5 = computeB5(ut);

  /* Pressure compensation */
  b6 = b5 - 4000;
  x1 = (_bmp085_coeffs.b2 * ((b6 * b6) >> 12)) >> 11;
  x2 = (_bmp085_coeffs.ac2 * b6) >> 11;
  x3 = x1 + x2;
  b3 = (((((int32_t)_bmp085_coeffs.ac1) * 4 + x3) << _bmp085Mode) + 2) >> 2;
  x1 = (_bmp085_coeffs.ac3 * b6) >> 13;
  x2 = (_bmp085_coeffs.b1 * ((b6 * b6) >> 12)) >> 16;
  x3 = ((x1 + x2) + 2) >> 2;
  b4 = (_bmp085_coeffs.ac4 * (uint32_t)(x3 + 32768)) >> 15;
  b7 = ((uint32_t)(up - b3) * (50000 >> _bmp085Mode));

  if (b7 < 0x80000000) {
    p = (b7 << 1) / b4;
  } else {
    p = (b7 / b4) << 1;
  }

  x1 = (p >> 8) * (p >> 8);
  x1 = (x1 * 3038) >> 16;
  x2 = (-7357 * p) >> 16;
  
  return p + ((x1 + x2 + 3791) >> 4);
}

/**************************************************************************/
/*!
    @brief  Reads the factory-set coefficients
*/
/**************************************************************************/
void BMP085::readCoefficients() {
#if BMP085_USE_DATASHEET_VALS
  _bmp085_coeffs.ac1 = 408;
  _bmp085_coeffs.ac2 = -72;
  _bmp085_coeffs.ac3 = -14383;
  _bmp085_coeffs.ac4 = 32741;
  _bmp085_coeffs.ac5 = 32757;
  _bmp085_coeffs.ac6 = 23153;
  _bmp085_coeffs.b1 = 6190;
  _bmp085_coeffs.b2 = 4;
  _bmp085_coeffs.mb = -32768;
  _bmp085_coeffs.mc = -8711;
  _bmp085_coeffs.md = 2868;
  _bmp085Mode = 0;
#else
  _bmp085_coeffs.ac1 = readS16(BMP085_REGISTER_CAL_AC1);
  _bmp085_coeffs.ac2 = readS16(BMP085_REGISTER_CAL_AC2);
  _bmp085_coeffs.ac3 = readS16(BMP085_REGISTER_CAL_AC3);
  _bmp085_coeffs.ac4 = read16(BMP085_REGISTER_CAL_AC4);
  _bmp085_coeffs.ac5 = read16(BMP085_REGISTER_CAL_AC5);
  _bmp085_coeffs.ac6 = read16(BMP085_REGISTER_CAL_AC6);
  _bmp085_coeffs.b1 = readS16(BMP085_REGISTER_CAL_B1);
  _bmp085_coeffs.b2 = readS16(BMP085_REGISTER_CAL_B2);
  _bmp085_coeffs.mb = readS16(BMP085_REGISTER_CAL_MB);
  _bmp085_coeffs.mc = readS16(BMP085_REGISTER_CAL_MC);
  _bmp085_coeffs.md = readS16(BMP085_REGISTER_CAL_MD);
#endif
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
int32_t BMP085::readRawTemperature() {
#if BMP085_USE_DATASHEET_VALS
  return 27898;
#else
  writeCommand(BMP085_REGISTER_CONTROL, BMP085_REGISTER_READTEMPCMD);
  _sleep_ms(5);
  return read16(BMP085_REGISTER_TEMPDATA);
#endif
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
int32_t BMP085::readRawPressure() {
#if BMP085_USE_DATASHEET_VALS
  return 23843;
#else
  uint8_t p8;
  uint16_t p16;
  int32_t p32;

  writeCommand(BMP085_REGISTER_CONTROL,
               BMP085_REGISTER_READPRESSURECMD + (_bmp085Mode << 6));
  switch (_bmp085Mode) {
  case BMP085_MODE_ULTRALOWPOWER:
    _sleep_ms(5);
    break;
  case BMP085_MODE_STANDARD:
    _sleep_ms(8);
    break;
  case BMP085_MODE_HIGHRES:
    _sleep_ms(14);
    break;
  case BMP085_MODE_ULTRAHIGHRES:
  default:
    _sleep_ms(26);
    break;
  }

  p16 = read16(BMP085_REGISTER_PRESSUREDATA);
  p32 = (uint32_t)p16 << 8;
  p8 = read8(BMP085_REGISTER_PRESSUREDATA + 2);
  p32 += p8;
  p32 >>= (8 - _bmp085Mode);

  return p32;
#endif
}

/**************************************************************************/
/*!
    @brief  Compute B5 coefficient used in temperature & pressure calcs.
*/
/**************************************************************************/
int32_t BMP085::computeB5(int32_t ut) {
  int32_t X1 =
      (ut - (int32_t)_bmp085_coeffs.ac6) * ((int32_t)_bmp085_coeffs.ac5) >> 15;
  int32_t X2 =
      ((int32_t)_bmp085_coeffs.mc << 11) / (X1 + (int32_t)_bmp085_coeffs.md);
  return X1 + X2;
}

/**************************************************************************/
/*!
    @brief  Reads an 8 bit value over I2C
*/
/**************************************************************************/
uint8_t BMP085::read8(uint8_t reg) {
  _i2c.beginTransmission(BMP085_ADDR);
  _i2c.write(reg);
  _i2c.endTransmission();
  _i2c.requestFrom(BMP085_ADDR, 1);
  auto retval = _i2c.read();
  _i2c.endTransmission();
  return retval;
}

/**************************************************************************/
/*!
    @brief  Reads a 16 bit value over I2C
*/
/**************************************************************************/
uint16_t BMP085::read16(uint8_t reg) {
  _i2c.beginTransmission(BMP085_ADDR);
  _i2c.write(reg);
  _i2c.endTransmission();
  _i2c.requestFrom(BMP085_ADDR, 2);
  uint16_t retval = (_i2c.read() << 8) | _i2c.read();
  _i2c.endTransmission();
  return retval;
}

/**************************************************************************/
/*!
    @brief  Reads a signed 16 bit value over I2C
*/
/**************************************************************************/
int16_t BMP085::readS16(uint8_t reg) {
  return static_cast<int16_t>(read16(reg));
}

/**************************************************************************/
/*!
    @brief  Writes an 8 bit value over I2C
*/
/**************************************************************************/
void BMP085::writeCommand(uint8_t reg, uint8_t value) {
  // _i2c.begin();
  _i2c.beginTransmission(BMP085_ADDR);
  _i2c.write(reg);
  _i2c.write(value);
  _i2c.endTransmission();
}