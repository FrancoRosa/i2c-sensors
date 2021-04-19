#include "zmod44xx.h"
#include "zmod44xx_config.h"

uint8_t i2c_addr;               /**< i2c address of the sensor */
uint8_t config[6];              /**< configuration parameter set */
uint8_t prod_data[5];           /**< production data */
uint16_t mox_lr;                /**< sensor specific parameter */
uint16_t mox_er;                /**< sensor specific parameter */
uint16_t pid;  

const uint8_t data_set_4450_continuous[] = {0x20, 0x04, 0x40, 0x09,
                                            0x03,
                                            0x00, 0x00, 0x80, 0x08};

const uint8_t data_set_4450i[] = {0x00, 0x28, 0xC3, 0xE3,
                                  0x00, 0x00, 0x80, 0x40};

const zmod44xx_conf meas_conf[]  = {
        .start = 0xC0,
        .h = {.addr = 0x40, .len = 2},
        .d = {.addr = 0x50, .len = 4, .data = &data_set_4450_continuous[0]},
        .m = {.addr = 0x60, .len = 1, .data = &data_set_4450_continuous[4]},
        .s = {.addr = 0x68, .len = 4, .data = &data_set_4450_continuous[5]},
        .r = {.addr = 0x99, .len = 2}

};

const zmod44xx_conf init_conf[] = {
        .start = 0x80,
        .h = {.addr = 0x40, .len = 2},
        .d = {.addr = 0x50, .len = 2, .data = &data_set_4450i[0]},
        .m = {.addr = 0x60, .len = 2, .data = &data_set_4450i[2]},
        .s = {.addr = 0x68, .len = 4, .data = &data_set_4450i[4]},
        .r = {.addr = 0x97, .len = 4}

};

int8_t zmod44xx_read_sensor_info(void)
{
    int8_t ret = 0;
    uint8_t data[ZMOD44XX_LEN_PID];
    uint8_t status = 0;
    uint8_t cmd = 0;
    uint16_t i = 0;

    /* wait for sensor ready */
    do {

        ret = write(i2c_addr, ZMOD44XX_ADDR_CMD, &cmd, 1);
        if(ret) {
            return ERROR_I2C;
        }

        delay_ms(200);

        ret = zmod44xx_read_status(&status);
        if(ret) {
            return ret;
        }
        i++;
    } while ((0x00 != (status & 0x80)) && (i < 1000));

    if (1000 <= i) {
        return ERROR_GAS_TIMEOUT;
    }

    ret = read(i2c_addr, ZMOD44XX_ADDR_PID, data, ZMOD44XX_LEN_PID);
    if(ret) {
        return ERROR_I2C;
    }
    pid = data[0] << 8 | data[1];

    ret = read(i2c_addr, ZMOD44XX_ADDR_CONF, config,
                    ZMOD44XX_LEN_CONF);
    if(ret) {
        return ERROR_I2C;
    }

    ret = read(i2c_addr, ZMOD44XX_ADDR_PROD_DATA, prod_data,
                    ZMOD44XX_LEN_PROD_DATA);
    if(ret) {
        return ERROR_I2C;
    }

    switch (pid) {

        case ZMOD4450_PID: {
#ifdef CONTINUOUS_MODE
            meas_conf = &zmod4450_continuous;
            init_conf = &zmod44xxi;
#endif
        } break;

        default: {
            return ERROR_SENSOR_UNSUPPORTED;
        } break;
    }

    return ZMOD44XX_OK;
}

int8_t zmod44xx_init_sensor(void)
{
    int8_t ret = 0;
    uint8_t data[4] = {0};

    if (!init_conf) {
        return ERROR_CONFIG_MISSING;
    }

    /* prepare sensor */
    ret = read(i2c_addr, 0xB7, data, 1);
    if(ret) {
        return ERROR_I2C;
    }

    float hspf = (-((float)config[2] * 256.0 + config[3]) *
                 ((config[4] + 640.0) * (config[5] + 80.0) - 512000.0))
                 / 12288000.0;

    if ((0.0 > hspf) || (4096.0 < hspf)) {
        return ERROR_INIT_OUT_OF_RANGE;
    }

    data[0] = (uint8_t)((uint16_t)hspf >> 8);
    data[1] = (uint8_t)((uint16_t)hspf & 0x00FF);

    ret = write(i2c_addr, init_conf->h.addr, data,
                     init_conf->h.len);
    if(ret) {
        return ERROR_I2C;
    }

    ret = write(i2c_addr, init_conf->d.addr,
                     init_conf->d.data, init_conf->d.len);
    if(ret) {
        return ERROR_I2C;
    }

    ret = write(i2c_addr, init_conf->m.addr,
                     init_conf->m.data, init_conf->m.len);
    if(ret) {
        return ERROR_I2C;
    }

    ret = write(i2c_addr, init_conf->s.addr,
                     init_conf->s.data, init_conf->s.len);
    if(ret) {
        return ERROR_I2C;
    }

    ret = write(i2c_addr, ZMOD44XX_ADDR_CMD, &init_conf->start, 1);
    if(ret) {
        return ERROR_I2C;
    }

    delay_ms(20);

    ret = zmod44xx_read_status(data);
    if (0x80 == (0x80 & data[0])) {
        return ERROR_GAS_TIMEOUT;
    }

    ret = read(i2c_addr, init_conf->r.addr, data,
                    init_conf->r.len);
    if(ret) {
        return ERROR_I2C;
    }

    mox_lr = (uint16_t)(data[0] << 8) | data[1];
    mox_er = (uint16_t)(data[2] << 8) | data[3];

    return ZMOD44XX_OK;
}

int8_t zmod44xx_init_measurement(void)
{
    int8_t ret = 0;
    uint8_t data[4] = {0};

    if (!meas_conf) {
        return ERROR_CONFIG_MISSING;
    }

    ret = read(i2c_addr, 0xB7, data, 1);
    if(ret) {
        return ERROR_I2C;
    }

    float hspf = (-((float)config[2] * 256.0 + config[3]) *
                 ((config[4] + 640.0) * (config[5] - 600.0) - 512000.0))
                 / 12288000.0;
    if ((0.0 > hspf) || (4096.0 < hspf)) {
        return ERROR_INIT_OUT_OF_RANGE;
    }

#ifdef CONTINUOUS_MODE
    data[0] = (uint8_t)((uint16_t)hspf >> 8);
    data[1] = (uint8_t)((uint16_t)hspf & 0x00FF);
#endif

    ret = write(i2c_addr, meas_conf->h.addr, data,
                     meas_conf->h.len);
    if(ret) {
        return ERROR_I2C;
    }

    ret = write(i2c_addr, meas_conf->d.addr,
                     meas_conf->d.data, meas_conf->d.len);
    if(ret) {
        return ERROR_I2C;
    }

    ret = write(i2c_addr, meas_conf->m.addr,
                     meas_conf->m.data, meas_conf->m.len);
    if(ret) {
        return ERROR_I2C;
    }

    ret = write(i2c_addr, meas_conf->s.addr,
                     meas_conf->s.data, meas_conf->s.len);
    if(ret) {
        return ERROR_I2C;
    }

    return ZMOD44XX_OK;
}

int8_t zmod44xx_start_measurement(void)
{
    int8_t ret = 0;

    ret = write(i2c_addr, ZMOD44XX_ADDR_CMD, &meas_conf->start, 1);
    if (ret) {
        return ERROR_I2C;
    }
    return ZMOD44XX_OK;
}

int8_t zmod44xx_read_status(uint8_t* status)
{
    int8_t ret;
    uint8_t st;

    ret = read(i2c_addr, ZMOD44XX_ADDR_STATUS, &st, 1);
    if (0 != ret) {
        return ret;
    }
    *status = st;
    return ZMOD44XX_OK;
}

int8_t zmod44xx_read_rmox(float* rmox)
{
    int8_t ret = 0;
    uint8_t data[2] = {0};
    uint16_t adc_result = 0;

    ret = read(i2c_addr, meas_conf->r.addr, data,
                    meas_conf->r.len);
    if (ret) {
        return ERROR_I2C;
    }

    adc_result = (uint16_t)(data[0] << 8) | data[1];

    ret = read(i2c_addr, 0xB7, data, 1);
    if (ret) {
        return ERROR_I2C;
    }

    if( 0 != data[0]) {
        if (STATUS_ACCESS_CONFLICT_MASK & data[0]) {
            return ERROR_ACCESS_CONFLICT;
        }
        else if (STATUS_POR_EVENT_MASK & data[0]) {
            return ERROR_POR_EVENT;
        }
    }

    if (0.0 > (adc_result - mox_lr)) {
        *rmox = 1e-3;
    } else if (0.0 >= (mox_er - adc_result)) {
        *rmox = 10e9;
    } else {
        *rmox = config[0] * 1000.0 * (adc_result - mox_lr) /
                                          (mox_er - adc_result);
    }

    return ZMOD44XX_OK;
}
