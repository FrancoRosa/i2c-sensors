/*******************************************************************************
 * Copyright (c) 2018 Integrated Device Technology, Inc.
 * All Rights Reserved.
 *
 * This code is proprietary to IDT, and is license pursuant to the terms and
 * conditions that may be accessed at:
 * https://www.idt.com/document/msc/idt-software-license-terms-gas-sensor-software
 *
 ******************************************************************************/

/**
 * @file    zmod44xx_types.h
 * @brief   ZMOD44xx types
 * @version 1.2.0
 * @author  IDT
 */

#ifndef _ZMOD44XX_TYPES_H
#define _ZMOD44XX_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
* @brief   function pointer type for i2c access
* @param   [in] addr 7-bit I2C slave address of the ZMOD44xx
* @param   [in] reg_addr address of internal register to read/write
* @param   [in,out] data pointer to the read/write data value
* @param   [in] len number of bytes to read/write
* @return  error code
* @retval  0 success
* @retval  "!= 0" error
*/
typedef int8_t (*zmod44xx_i2c_ptr_t)(uint8_t addr, uint8_t reg_addr,
                                     const uint8_t* data, uint8_t len);

/**
 * @brief function pointer to hardware dependent delay function
 */
typedef void (*zmod44xx_delay_ptr_p)(uint32_t ms);

/**
 * \defgroup sensor_ids Gas sensor IDs
 * The gas sensor product IDs.
 * @{
 */

#define ZMOD4450_PID             (0x7310)
/** @} */

#define ZMOD44XX_OK                 (0) /**< Return value if no fault has been found. */

 /**
 * \defgroup error_codes Error codes
 * The gas sensor and API error codes.
 * @{
 */

#define ERROR_INIT_OUT_OF_RANGE     (1) /**< The initialize value is out of range. */
#define ERROR_GAS_TIMEOUT           (2) /**< The operation took too long. */
#define ERROR_I2C                   (3) /**< Failure in i2c communication. */
#define ERROR_SENSOR_UNSUPPORTED    (4) /**< Sensor is not supported with this firmware. */
#define ERROR_CONFIG_MISSING        (5) /**< There is no pointer to a valid configuration. */
#define ERROR_SENSOR                (6) /**< Sensor malfunction. */
#define ERROR_ACCESS_CONFLICT       (7) /**< AccessConflict. */
#define ERROR_POR_EVENT             (8) /**< POR_event. */
/** @} */

/**
 * @brief A single data set for the configuration
 */
typedef struct {
    uint8_t addr;
    uint8_t len;
    const uint8_t* data;
} zmod44xx_conf_str;

/**
 * @brief Structure to hold the gas sensor module configuration.
 */
typedef struct {
    uint8_t start;
    zmod44xx_conf_str h;
    zmod44xx_conf_str d;
    zmod44xx_conf_str m;
    zmod44xx_conf_str s;
    zmod44xx_conf_str r;
} zmod44xx_conf;

/**
 * @brief Device structure ZMOD44xx
 */
 typedef struct {
     uint8_t i2c_addr;               /**< i2c address of the sensor */
     uint8_t config[6];              /**< configuration parameter set */
     uint8_t prod_data[5];           /**< production data */
     uint16_t mox_lr;                /**< sensor specific parameter */
     uint16_t mox_er;                /**< sensor specific parameter */
     uint16_t pid;                   /**< product id of the sensor */
     zmod44xx_i2c_ptr_t read;        /**< function pointer to i2c read */
     zmod44xx_i2c_ptr_t write;       /**< function pointer to i2c write */
     zmod44xx_delay_ptr_p delay_ms;  /**< function pointer to delay function */
     const zmod44xx_conf* init_conf; /**< pointer to the initialize configuration */
     const zmod44xx_conf* meas_conf; /**< pointer to the measurement configuration */
 } zmod44xx_dev_t;

#ifdef __cplusplus
}
#endif

 #endif // _ZMOD44XX_TYPES_H
