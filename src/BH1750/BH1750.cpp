/*

  This is a library for the BH1750FVI Digital Light Sensor breakout board.

  The BH1750 board uses I2C for communication. Two pins are required to
  interface to the device. Configuring the I2C bus is expected to be done
  in user code. The BH1750 library doesn't do this automatically.

  Written by Christopher Laws, March, 2013.

*/

#include "sensor/BH1750.h"
// #include <stdio.h>
#include "sensor/Wire.h"

// #define _sleep_us busy_wait_us
// #define _sleep_ms busy_wait_ms
#define _sleep_us sleep_us
#define _sleep_ms sleep_ms

/**
 * Constructor
 * @params addr Sensor address (0x76 or 0x72, see datasheet)
 *
 * On most sensor boards, it was 0x76
 */
BH1750::BH1750(uint8_t addr) {
  BH1750_I2CADDR = addr;
}

bool BH1750::begin(i2c_inst_t* i2c, Mode mode) {
  wire.begin(i2c);

  // Configure sensor in specified mode
  return configure(mode);
}


/**
 * Configure BH1750 with specified mode
 * @param mode Measurement mode
 */
bool BH1750::configure(Mode mode) {
  BH1750_MODE= mode;
  // default transmission result to a value out of normal range
  uint8_t ack = 5;

  // Check measurement mode is valid
  switch (mode) {

    case BH1750::CONTINUOUS_HIGH_RES_MODE:
    case BH1750::CONTINUOUS_HIGH_RES_MODE_2:
    case BH1750::CONTINUOUS_LOW_RES_MODE:
    case BH1750::ONE_TIME_HIGH_RES_MODE:
    case BH1750::ONE_TIME_HIGH_RES_MODE_2:
    case BH1750::ONE_TIME_LOW_RES_MODE:
      wire.beginTransmission(BH1750_I2CADDR);
      // Send mode to sensor
      wire.write((uint8_t)BH1750_MODE);
      wire.endTransmission();

      // Wait a few moments to wake up
      _sleep_ms(10);
      break;

    default:
      // Invalid measurement mode
      // printf("[BH1750] ERROR: Invalid mode\n");
      break;

  }
  error_str = "";
  // Check result code
  switch (ack) {
    case 0:
      BH1750_MODE = mode;
      return true;
    case 1: // too long for transmit buffer
      error_str = "[BH1750] ERROR: too long for transmit buffer";
      break;
    case 2: // received NACK on transmit of address
      error_str = "[BH1750] ERROR: received NACK on transmit of address";
      break;
    case 3: // received NACK on transmit of data
      error_str = "[BH1750] ERROR: received NACK on transmit of data";
      break;
    case 4: // other error
      error_str = "[BH1750] ERROR: other error";
      break;
    default:
      error_str = "[BH1750] ERROR: undefined error";
      break;
  }

  return false;

}


void BH1750::powerDown(){
  wire.beginTransmission(BH1750_I2CADDR);
  wire.write(BH1750_POWER_DOWN);
  wire.endTransmission();

}

void BH1750::powerUp(){
  wire.beginTransmission(BH1750_I2CADDR);
  wire.write(BH1750_POWER_ON);
  wire.endTransmission();
}


/**
 * Read light level from sensor
 * @return Light level in lux (0 ~ 65535)
 */
uint16_t BH1750::readLightLevel(bool maxWait) {

  if (BH1750_MODE == UNCONFIGURED) {
    error_str = "[BH1750] Device is not configured!";
    return 0;
  }

  // Measurement result will be stored here
  uint16_t level=65535;

  // Send mode to sensor
  wire.beginTransmission(BH1750_I2CADDR);
  wire.write((uint8_t)BH1750_MODE);
  wire.endTransmission();

  // Wait for measurement to be taken.
  // Measurements have a maximum measurement time and a typical measurement
  // time. The maxWait argument determines which measurement wait time is
  // used when a one-time mode is being used. The typical (shorter)
  // measurement time is used by default and if maxWait is set to True then
  // the maximum measurement time will be used. See data sheet pages 2, 5
  // and 7 for more details.
  // A continuous mode measurement can be read immediately after re-sending
  // the mode command.

  switch (BH1750_MODE) {

    case BH1750::ONE_TIME_LOW_RES_MODE:
    case BH1750::ONE_TIME_HIGH_RES_MODE:
    case BH1750::ONE_TIME_HIGH_RES_MODE_2:

      if (BH1750_MODE == BH1750::ONE_TIME_LOW_RES_MODE) {
        maxWait ? _sleep_ms(24) : _sleep_ms(16);
      }
      else {
        maxWait ? _sleep_ms(180) :_sleep_ms(120);
      }
      break;
    default: break;
  }

  // Read two uint8_ts from the sensor, which are low and high parts of the sensor
  // value
  wire.requestFrom(BH1750_I2CADDR, 2);
  if (wire.available() == 2) {
    level = wire.read();
    level <<= 8;
    level |= wire.read();
  }

  // Print raw value if debug enabled
  #ifdef BH1750_DEBUG
  printf("[BH1750] Raw value: %d", level);
  #endif

  // Convert raw value to lux
  level /= 1.2;

  // Print converted value if debug enabled
  #ifdef BH1750_DEBUG
  printf("[BH1750] Converted value: \n", level);
  #endif

  return level;

}