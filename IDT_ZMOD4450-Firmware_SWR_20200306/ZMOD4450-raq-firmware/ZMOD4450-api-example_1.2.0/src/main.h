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
 * @file    main.h
 * @brief   This is an example for the ZMOD4450 gas sensor module.
 * @version 1.2.0
 * @author  IDT
 */

// #include <conio.h> /* This include is just for Windows PCs. */

/* Files needed for hardware access, needs to be adjusted to target. */
// #include "../dependencies/hicom/hicom.h"
// #include "../dependencies/hicom/hicom_i2c.h"

#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port

/* files to control the sensor */
#include "continuous.h"

/* File needed for the cleaning procedure */
#include "zmod44xx_cleaning.h"
