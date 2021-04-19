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
 * @file    continuous.h
 * @brief   ZMOD44xx continuous mode
 * @version 1.2.0
 * @author  IDT
 */

#ifndef CONTINUOUS_H_
#define CONTINUOUS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

/* Algorithm library header files, download the target specific library
 * from the IDT webpage. */
#include "raq.h"

#include "zmod44xx_types.h"
#include "zmod44xx.h"

// start sequencer defines

#define FIRST_SEQ_STEP      0
#define LAST_SEQ_STEP       1

/**
* @brief   function for execution of initial continuous mode sequence steps
* @param   [in] dev pointer to the device structure ZMOD44xx
* @return  error code
* @retval  0 success
* @retval  "!= 0" error
*/
int8_t cont_init(zmod44xx_dev_t* dev);

/**
* @brief   function for execution of continuous mode sequence steps
* @param   [in] dev pointer to the device structure ZMOD44xx
* @return  error code
* @retval  0 success
* @retval  "!= 0" error
*/
int8_t cont_run(zmod44xx_dev_t* dev);

#ifdef __cplusplus
}
#endif

#endif /* CONTINUOUS_H_ */
