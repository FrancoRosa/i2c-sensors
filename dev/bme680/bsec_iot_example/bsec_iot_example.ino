#include <Wire.h>
#include "bsec_datatypes.h"
#include "bsec_interface.h"

int8_t bus_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data_ptr, uint16_t data_len) {
  Wire.beginTransmission(dev_addr);
  Wire.write(reg_addr);
  /* Set register address to start writing to */
  /* Write the data */
  for (int index = 0; index < data_len; index++) {
    Wire.write(reg_data_ptr[index]);
  }
  return (int8_t)Wire.endTransmission();
}

int8_t bus_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data_ptr, uint16_t data_len) {
  int8_t comResult = 0;
  Wire.beginTransmission(dev_addr);
  Wire.write(reg_addr);
  /* Set register address to start reading from */
  comResult = Wire.endTransmission();
  delayMicroseconds(150);
  /* Precautionary response delay */
  Wire.requestFrom(dev_addr, (uint8_t)data_len);
  /* Request data */
  int index = 0;
  while (Wire.available()) { /* The slave device may send less than requested (burst read) */
    reg_data_ptr[index] = Wire.read();
    index++;
  }
  return comResult;
}

void sleep(uint32_t t_ms) {
  delay(t_ms);
}

int64_t get_timestamp_us() {
 return (int64_t) millis() * 1000;
}

void output_ready(int64_t timestamp, float iaq, uint8_t iaq_accuracy, float temperature, float humidity,
float pressure, float raw_temperature, float raw_humidity, float gas,
bsec_library_return_t bsec_status) {
  Serial.print("[");
  Serial.print(timestamp/1e6);
  Serial.print("] T: ");
  Serial.print(temperature);
  Serial.print("| rH: ");
  Serial.print(humidity);
  Serial.print("| IAQ: ");
  Serial.print(iaq);
  Serial.print(" (");
  Serial.print(iaq_accuracy);
  Serial.println(")");
}

void setup() {
  return_values_init ret;
  /* Init I2C and serial communication */
  Wire.begin();
  Serial.begin(115200);
  /* Call to the function which initializes the BSEC library
  * Switch on low-power mode and provide no temperature offset */
  ret = bsec_iot_init(BSEC_SAMPLE_RATE_LP, 0.0f, bus_write, bus_read, sleep,
  state_load, config_load);
  if (ret.bme680_status) {
    /* Could not intialize BME680 */
    Serial.println("Error while initializing BME680");
    return;
  } else if (ret.bsec_status) {
    /* Could not intialize BSEC library */
    Serial.println("Error while initializing BSEC library");
    return;
  }
  /* Call to endless loop function which reads and processes data based on sensor settings */
  /* State is saved every 10.000 samples, which means every 10.000 * 3 secs = 500 minutes */
  bsec_iot_loop(sleep, get_timestamp_us, output_ready, state_save, 10000);
}

void loop() {
  /* We do not need to put anything here as we enter our own loop function in setup() */
}