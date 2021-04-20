#ifndef _ARDUINO_PORT_I2C_H
#define _ARDUINO_PORT_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

#include <Wire.h>
#include "zmod44xx.h"
#include "zmod44xx_types.h"

int8_t arduino_i2c_read(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *buf, uint8_t len)
{
  Wire.beginTransmission(i2c_addr);
  Wire.write(reg_addr);
  if (Wire.endTransmission() != 0)
  {
    Serial.print("NACK");
    return 3; //Error: Sensor did not ack
  }

  Wire.requestFrom(static_cast<uint8_t>(i2c_addr), len);
  for (uint8_t i = 0; i < len; i++)
  {
    *(buf + i) = Wire.read();
  }
  return 0;
}


int8_t arduino_i2c_write(uint8_t i2c_addr, uint8_t reg_addr, const uint8_t *buf, uint8_t len)
{
  Wire.beginTransmission(i2c_addr);
  Wire.write(reg_addr);
  for (uint8_t i = 0; i < len; i++)
  {
    Wire.write(buf[i]);
  }

  if (Wire.endTransmission() != 0)
  {
    Serial.print("NACK");
    return 3; //Error: Sensor did not ack
  }
  return 0;
}

#ifdef __cplusplus
}
#endif

#endif // __ZMOD4XXX_TARGET_SPECIFIC__