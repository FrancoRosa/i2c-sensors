/*******************************************************************************
 * Copyright (c) 2019 Integrated Device Technology, Inc.
 * All Rights Reserved.
 *
 * This code is proprietary to IDT, and is license pursuant to the terms and
 * conditions that may be accessed at:
 * https://www.idt.com/document/msc/idt-software-license-terms-gas-sensor-software
 *
 ******************************************************************************/

/**
 * @file    zmod44xx_cleaning.h
 * @date    2019-10-28
 * @author  IDT
 * @version 1.0.1 - https://semver.org/
 * @brief   This file contains the cleaning function definition for ZMOD44xx.
 * @details The library contains the function that starts the cleaning procedure.
 *          **The procedure takes around 10 minutes.** After successful cleaning, 
 *          the function return 0. **The procedure can be run only once.**
 */

#ifndef ZMOD44XX_CLEANING_H_
#define ZMOD44XX_CLEANING_H_

#include <stdint.h>
#include "zmod44xx_types.h"

#define ERROR_CLEANING (9) /**< Error cleaning. */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Start a cleaning procedure (**Around 10 minutes blocking.**).
 * @param   [in]  dev Pointer to the device.
 * @return  Error code.
 * @retval  0 Success.
 * @retval  "!= 0" Error.
 */
uint8_t zmod44xx_cleaning_run(zmod44xx_dev_t *dev);

#ifdef __cplusplus
}
#endif

#endif /* ZMOD44XX_CLEANING_H_ */
