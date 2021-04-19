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
 * @file    zmod44xx_config.h
 * @brief   ZMOD44xx configuration
 * @version 1.2.0
 * @author  IDT
 */

#ifndef _ZMOD44XX_CONFIG_H
#define _ZMOD44XX_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "zmod44xx_types.h"

/* ZMOD4450 measurement configuration for continuous mode*/
const uint8_t data_set_4450_continuous[] = {0x20, 0x04, 0x40, 0x09,
                                            0x03,
                                            0x00, 0x00, 0x80, 0x08};

/* ZMOD4450 initialization configuration */
const uint8_t data_set_4450i[] = {0x00, 0x28, 0xC3, 0xE3,
                                  0x00, 0x00, 0x80, 0x40};

/**
 * @brief ZMOD4450 configuration for continuous mode
 */
const zmod44xx_conf zmod4450_continuous  = {
        .start = 0xC0,
        .h = {.addr = 0x40, .len = 2},
        .d = {.addr = 0x50, .len = 4, .data = &data_set_4450_continuous[0]},
        .m = {.addr = 0x60, .len = 1, .data = &data_set_4450_continuous[4]},
        .s = {.addr = 0x68, .len = 4, .data = &data_set_4450_continuous[5]},
        .r = {.addr = 0x99, .len = 2}

};

/**
 * @brief ZMOD44XX sensor initialization configuration
 */
const zmod44xx_conf zmod44xxi = {
        .start = 0x80,
        .h = {.addr = 0x40, .len = 2},
        .d = {.addr = 0x50, .len = 2, .data = &data_set_4450i[0]},
        .m = {.addr = 0x60, .len = 2, .data = &data_set_4450i[2]},
        .s = {.addr = 0x68, .len = 4, .data = &data_set_4450i[4]},
        .r = {.addr = 0x97, .len = 4}

};

#ifdef __cplusplus
}
#endif

#endif //  _ZMOD44XX_CONFIG_H
