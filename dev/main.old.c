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
 * @file    main.c
 * @brief   This is an example for the ZMOD4450 gas sensor module.
 * @version 1.2.0
 * @author  IDT
 **/

#include "main.h"
/* forward declaration */
void print_hicom_error(hicom_status_t status);

int main()
{
    /* These are the hardware handles which needs to be adjusted to specific HW */
    hicom_handle_t hicom_handle;
    hicom_status_t hicom_status;

    zmod44xx_dev_t dev;
    int8_t ret;

    /* ****** BEGIN TARGET SPECIFIC INITIALIZATION ************************** */
    /* This part automatically resets the sensor. */
    /* connect to HiCom board */
    hicom_status = hicom_open(&hicom_handle);
    if (FTC_SUCCESS != hicom_status) {
        print_hicom_error(hicom_status);
        return hicom_status;
    }

    /* switch supply on */
    hicom_status = hicom_power_on(hicom_handle);
    if (FTC_SUCCESS != hicom_status) {
        print_hicom_error(hicom_status);
        return hicom_status;
    }

    set_hicom_handle(&hicom_handle);

    /* ****** END TARGET SPECIFIC INITIALIZATION **************************** */

    /* Set initial hardware parameter */
    dev.read = hicom_i2c_read;
    dev.write = hicom_i2c_write;
    dev.delay_ms = hicom_sleep;
    dev.i2c_addr = ZMOD4450_I2C_ADDRESS;

    /**
    * @brief   This function starts the cleaning procedure. It's
    * recommended to be executed after product assembly. This
    * helps to clean the metal oxide surface from assembly.
    * IMPORTANT NOTE: The cleaning procedure can be run only once
    * during the modules lifetime and takes 10 minutes.
    * @param   [in] dev pointer to the device structure ZMOD44xx
    * @return  error code
    * @retval  0 success
    * @retval  "!= 0" error
    */
    //ret = zmod44xx_cleaning_run(&dev);
    //if (ret) {
    //    printf("Error %d, exiting program!\n", ret);
    //    goto exit;
    //}

    /* initialize and start sensor */
    ret = zmod44xx_read_sensor_info(&dev);
    if(ret) {
        printf("Error %d, exiting program!\n", ret);
        goto exit;
    }

    ret = zmod44xx_init_sensor(&dev);
    if(ret) {
        printf("Error %d, exiting program!\n", ret);
        goto exit;
    }

    ret = zmod44xx_init_measurement(&dev);
    if(ret) {
        printf("Error %d, exiting program!\n", ret);
        goto exit;
    }

#ifdef CONTINUOUS_MODE
    ret = cont_init(&dev);
    if(ret) {
        printf("Error %d, exiting program!\n", ret);
        goto exit;
    }

    printf("Evaluate measurements in a loop. Press any key to quit.\n");
    do {
        ret = cont_run(&dev);
        if(ret) {
            printf("Error %d, exiting program!\n", ret);
            goto exit;
        }
    /* kbhit() is a windows specific function and needs to be removed for
     * other hardware. */
    } while (!kbhit());
#endif

exit:
    /* ****** BEGIN TARGET SPECIFIC DEINITIALIZATION ****** */
    hicom_status = hicom_power_off(hicom_handle);
    if (FTC_SUCCESS != hicom_status) {
        print_hicom_error(hicom_status);
        return hicom_status;
    }

    /* disconnect HiCom board */
    hicom_status = hicom_close(hicom_handle);
    if (FTC_SUCCESS != hicom_status) {
        print_hicom_error(hicom_status);
        return hicom_status;
    }
    /* ****** END TARGET SPECIFIC DEINITIALIZATION ****** */

    printf("Bye!\n");
    return 0;
}

void print_hicom_error(hicom_status_t status)
{
    char error_str[512];
    hicom_get_error_string(status, error_str, sizeof error_str);
    fprintf(stderr, "ERROR (HiCom): %s\n", error_str);
}
