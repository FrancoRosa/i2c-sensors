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
 * @file    zmod44xx.h
 * @brief   ZMOD44xx functions
 * @version 1.2.0
 * @author  IDT
 */

 #ifndef _ZMOD44XX_H
 #define _ZMOD44XX_H

#ifdef __cplusplus
extern "C" {
#endif

#include "zmod44xx_types.h"

#define ZMOD4450_I2C_ADDRESS        (0x32)

#define ZMOD44XX_ADDR_PID           (0x00)
#define ZMOD44XX_ADDR_CONF          (0x20)
#define ZMOD44XX_ADDR_PROD_DATA     (0x26)
#define ZMOD44XX_ADDR_CMD           (0x93)
#define ZMOD44XX_ADDR_STATUS        (0x94)

#define ZMOD44XX_LEN_PID            (2)
#define ZMOD44XX_LEN_CONF           (6)
#define ZMOD44XX_LEN_PROD_DATA      (5)

#define STATUS_SEQUENCER_RUNNING_MASK     (0x80) /**< Sequencer is running */
#define STATUS_SLEEP_TIMER_ENABLED_MASK   (0x40) /**< SleepTimer_enabled */
#define STATUS_ALARM_MASK                 (0x20) /**< Alarm */
#define STATUS_LAST_SEQ_STEP_MASK         (0x1F) /**< Last executed sequencer step */
#define STATUS_POR_EVENT_MASK             (0x80) /**< POR_event */
#define STATUS_ACCESS_CONFLICT_MASK       (0x40) /**< AccessConflict */

/**
 * @brief   Read sensor parameter.
 * @param   [in] dev pointer to the device
 * @return  error code
 * @retval  0 success
 * @retval  "!= 0" error
 * @note    This function must be called once before running other sensor
 *          functions.
 */
int8_t zmod44xx_read_sensor_info(zmod44xx_dev_t* dev);

/**
 * @brief   Initialize the sensor after power on.
 * @param   [in] dev pointer to the device
 * @return  error code
 * @retval  0 success
 * @retval  "!= 0" error
 */
int8_t zmod44xx_init_sensor(zmod44xx_dev_t* dev);

/**
 * @brief   Initialize the sensor for zmod4450 measurement.
 * @param   [in] dev pointer to the device
 * @return  error code
 * @retval  0 success
 * @retval  "!= 0" error
 */
int8_t zmod44xx_init_measurement(zmod44xx_dev_t* dev);

/**
 * @brief   Start the measurement.
 * @param   [in] dev pointer to the device
 * @return  error code
 * @retval  0 success
 * @retval  "!= 0" error
 */
int8_t zmod44xx_start_measurement(zmod44xx_dev_t* dev);

/**
 * @brief   Read the status of the device.
 * @param   [in] dev pointer to the device
 * @param   [in,out] status pointer to the status variable
 * @return  error code
 * @retval  0 success
 * @retval  "!= 0" error
 */
int8_t zmod44xx_read_status(zmod44xx_dev_t* dev, uint8_t* status);

/**
 * @brief   Read adc values from sensor and calculate RMOX
 * @param   [in] dev pointer to the device
 * @param   [in,out] rmox pointer to the resulting Rmox value
 * @return  error code
 * @retval  0 success
 * @retval  "!= 0" error
 */
int8_t zmod44xx_read_rmox(zmod44xx_dev_t* dev, float* rmox);

#ifdef __cplusplus
}
#endif

#endif //  _ZMOD44XX_H
