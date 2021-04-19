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
 * @file    continuous.c
 * @brief   ZMOD44xx continuous mode
 * @version 1.2.0
 * @author  IDT
 **/

#include "continuous.h"

static raq_params raq_par = {
        .alpha = 0.8,
        .stop_delay = 24,
        .threshold = 1.3,
        .tau = 720,
        .stabilization_samples = 15,
};

raq_results_t raq_results = { .cs_state = OFF, .conc_ratio = 0.0F };

int8_t cont_init(zmod44xx_dev_t* dev)
{
    int8_t ret;
    uint8_t zmod44xx_status;

    ret = zmod44xx_start_measurement(dev);
    if(ret) {
        printf("Error %d, exiting program!\n", ret);
        return ret;
    }

    /* Wait for initialization finished. */
    do {
        dev->delay_ms(50);
        ret = zmod44xx_read_status(dev, &zmod44xx_status);
        if(ret) {
            printf("Error %d, exiting program!\n", ret);
            return ret;
        }
    } while (FIRST_SEQ_STEP != (zmod44xx_status & STATUS_LAST_SEQ_STEP_MASK));

    return ZMOD44XX_OK;
}

int8_t cont_run(zmod44xx_dev_t* dev)
{
    float r_mox;
    uint8_t zmod44xx_status;
    uint8_t state;
    int8_t ret;

    /* INSTEAD OF POLLING THE INTERRUPT CAN BE USED FOR OTHER HW */
    /* wait until readout result is possible */
    ret = zmod44xx_read_status(dev, &zmod44xx_status);
    if(ret) {
        printf("Error %d, exiting program!\n", ret);
        return ret;
    }
    if(LAST_SEQ_STEP != (zmod44xx_status & STATUS_LAST_SEQ_STEP_MASK))
    {
        dev->delay_ms(50);
        return ZMOD44XX_OK;
    }

    /* evaluate and show measurement results */
    ret = zmod44xx_read_rmox(dev, &r_mox);
    if(ret) {
        if(ERROR_ACCESS_CONFLICT == ret) {
            printf("Error %d, AccessConflict!\n", ret);
            return ZMOD44XX_OK;
        }
        else if(ERROR_POR_EVENT == ret) {
            printf("Error %d, exiting program!\n", ret);
            return ret;
        }
    }

    /* To work with the algorithms target specific libraries needs to be
     * downloaded from IDT webpage and included into the project */

    /* get raq control signal and Air Quality Change Rate */
    state = calc_raq(r_mox, &raq_par, &raq_results);

    printf("\n");
    printf("Measurement:\n");
    printf("  Rmox  = %5.0f kOhm\n", (r_mox / 1000.0));
    if (ZMOD4450_OK == state) {
        printf("  raq: control state %d\n", raq_results.cs_state);
        printf("  raq: Air Quality Change Rate %f\n", raq_results.conc_ratio);
    }
    else {
        printf("  raq: control state: Sensor not stabilized\n");
        printf("  raq: Air Quality Change Rate: Sensor not stabilized\n");
    }

    /* INSTEAD OF POLLING THE INTERRUPT CAN BE USED FOR OTHER HW */
    /* waiting for sensor ready */
    while (FIRST_SEQ_STEP != (zmod44xx_status & STATUS_LAST_SEQ_STEP_MASK)) {
        dev->delay_ms(50);
        ret = zmod44xx_read_status(dev, &zmod44xx_status);
        if(ret) {
            printf("Error %d, exiting program!\n", ret);
            return ret;
        }
    }

    return ZMOD44XX_OK;
}
