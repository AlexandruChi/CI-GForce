#include "sensor_thread.h"
#include "MPU6500.h"
#include <math.h>
#include <stdio.h>
#include "i2c.h"

#include "sensor_data.h"

#define I2C_TIMEOUT 100
#define I2C_RETRIES 100

#define WAIT_TIME 1

ssp_err_t init_MPU6500(const sf_i2c_instance_t *device, uint32_t init_time);
ssp_err_t check_MPU6500(const sf_i2c_instance_t *device, uint8_t *new_data);
ssp_err_t read_data_MPU6500(const sf_i2c_instance_t *device, double *data);

#define STATE_WAIT 0
#define STATE_CHECK 1
#define STATE_READ 2
#define STATE_WRITE 3
#define STATE_CHANGE_UNIT 4
#define STATE_ERROR ((uint8_t)-1)

sensor_data_t sensor_data;

#define G_TO_M 9.80665

#define LIMIT_ZERO 0.25
#define LIMIT_LOW 0.75
#define LIMIT_NORMAL 1.25
#define LIMIT_HIGH 1.75

#define DEFAULT_UNIT UNIT_M

TX_SEMAPHORE* get_data_semaphore() {
    return &g_data_semaphore;
}

TX_SEMAPHORE* get_unit_semaphore() {
    return &g_unit_semaphore;
}

TX_SEMAPHORE* get_time_semaphore() {
    return &g_time_semaphore;
}

TX_MUTEX* get_data_mutex() {
    return &g_data_mutex;
}

void sensor_thread_entry(void) {
    ssp_err_t ret = SSP_SUCCESS;

    volatile uint32_t time_cy;
    volatile float time_us = 0;
    time_us++;

    // wait for the other thread to start
    tx_thread_sleep(100);

    tx_semaphore_ceiling_put(&g_time_semaphore, 1);
    tx_thread_relinquish();
    time_cy = DWT->CYCCNT;
    time_us = (float)time_cy / 240;
    __NOP();

    if (SSP_SUCCESS != (ret = g_sf_i2c_device_sensor.p_api->open(g_sf_i2c_device_sensor.p_ctrl, g_sf_i2c_device_sensor.p_cfg))) {
        while(1);
    }

    if (SSP_SUCCESS != (ret = init_MPU6500(&g_sf_i2c_device_sensor, 100))) {
        while(1);
    }

    if (SSP_SUCCESS != (ret = g_sf_external_irq_sensor.p_api->open(g_sf_external_irq_sensor.p_ctrl, g_sf_external_irq_sensor.p_cfg))) {
        while(1);
    }

    uint8_t unit = DEFAULT_UNIT;
    uint8_t new_data;
    double data[3];

    uint8_t state = STATE_WAIT;

    while (1) {
        switch (state) {
            case STATE_WAIT:
                if (SSP_SUCCESS == g_sf_external_irq_sensor.p_api->wait(g_sf_external_irq_sensor.p_ctrl, WAIT_TIME)) {
                    state = STATE_CHECK;
                    break;
                }

                if (!tx_semaphore_get(&g_unit_semaphore, WAIT_TIME)) {
                    state = STATE_CHANGE_UNIT;
                    break;
                }

                break;

            case STATE_CHECK:
                if (SSP_SUCCESS != (ret = check_MPU6500(&g_sf_i2c_device_sensor, &new_data))) {
                    state = STATE_ERROR;
                    break;
                }

                if (new_data) {
                    state = STATE_READ;
                    break;
                }

                state = STATE_WAIT;
                break;

            case STATE_READ:
                if (SSP_SUCCESS != (ret = read_data_MPU6500(&g_sf_i2c_device_sensor, data))) {
                    state = STATE_ERROR;
                    break;
                }

                state = STATE_WRITE;
                break;

            case STATE_WRITE:
                tx_mutex_get(&g_data_mutex, TX_WAIT_FOREVER);

                sensor_data.axis.x = data[0];
                sensor_data.axis.y = data[1];
                sensor_data.axis.z = data[2];

                sensor_data.accel = sqrt(
                        sensor_data.axis.x * sensor_data.axis.x +
                        sensor_data.axis.y * sensor_data.axis.y +
                        sensor_data.axis.z * sensor_data.axis.z
                );

                if (sensor_data.accel > LIMIT_HIGH) {
                     sensor_data.level = LEVEL_MAX;
                } else if (sensor_data.accel > LIMIT_NORMAL) {
                    sensor_data.level = LEVEL_HIGH;
                } else if (sensor_data.accel > LIMIT_LOW) {
                    sensor_data.level = LEVEL_NORMAL;
                } else if (sensor_data.accel > LIMIT_ZERO){
                    sensor_data.level  = LEVEL_LOW;
                } else {
                    sensor_data.level = LEVEL_ZERO;
                }

                sensor_data.unit = unit;
                switch (sensor_data.unit) {
                    case UNIT_M:

                        sensor_data.accel *= G_TO_M;
                        sensor_data.axis.x *= G_TO_M;
                        sensor_data.axis.y *= G_TO_M;
                        sensor_data.axis.z *= G_TO_M;

                        break;

                    case UNIT_G:
                        break;

                    default:
                        break;
                }

                tx_mutex_put(&g_data_mutex);
                tx_semaphore_ceiling_put(&g_data_semaphore, 1);

                state = STATE_WAIT;
                break;

            case STATE_CHANGE_UNIT:
                switch (unit) {
                    case UNIT_M:
                        unit  = UNIT_G;
                        break;

                    case UNIT_G:
                        unit = UNIT_M;
                        break;

                    default:
                        unit = UNIT_M;
                        break;
                }

                state = STATE_WRITE;
                break;

            case STATE_ERROR:
                state = STATE_WAIT;
                break;

            default:
                state = STATE_WAIT;
                break;
        }
    }
}

ssp_err_t init_MPU6500(const sf_i2c_instance_t *device, uint32_t init_time) {
    ssp_err_t ret = SSP_SUCCESS;

    for (size_t i = 0; i < I2C_RETRIES; i++) {
        device->p_api->lockWait(g_sf_i2c_device_sensor.p_ctrl, TX_WAIT_FOREVER);
        ret = i2c_write_register(device, PWR_MGMT_1, 1 << DEVICE_RESET, I2C_TIMEOUT);
        device->p_api->unlock(g_sf_i2c_device_sensor.p_ctrl);
        if (SSP_SUCCESS != ret) {
            continue;
        }

        break;
    }

    if (ret != SSP_SUCCESS) {
        return ret;
    }

    tx_thread_sleep(init_time);

    for (size_t i = 0; i < I2C_RETRIES; i++) {
        device->p_api->lockWait(g_sf_i2c_device_sensor.p_ctrl, TX_WAIT_FOREVER);
        ret = i2c_write_register(device, PWR_MGMT_1, 1 << TEMP_DIS, I2C_TIMEOUT);
        ret = i2c_write_register(device, PWR_MGMT_2, 1 << DISABLE_XG | 1 << DISABLE_YG | 1 << DISABLE_ZG, I2C_TIMEOUT);
        ret = i2c_write_register(device, CONFIG, 1 << DLPF_CFG, I2C_TIMEOUT);
        ret = i2c_write_register(device, SMPLRT_DIV, 99, I2C_TIMEOUT);
        ret = i2c_write_register(device, INT_ENABLE, 1 << RAW_RDY_EN, I2C_TIMEOUT);
        device->p_api->unlock(g_sf_i2c_device_sensor.p_ctrl);
        if (SSP_SUCCESS != ret) {
            continue;
        }

        break;
    }

    return ret;
}

ssp_err_t check_MPU6500(const sf_i2c_instance_t *device, uint8_t *new_data) {
    ssp_err_t ret = SSP_SUCCESS;
    uint8_t byte;

    for (size_t i = 0; i < I2C_RETRIES; i++) {
        device->p_api->lockWait(g_sf_i2c_device_sensor.p_ctrl, TX_WAIT_FOREVER);
        ret = i2c_read_register(device, INT_STATUS, &byte, I2C_TIMEOUT);
        device->p_api->unlock(g_sf_i2c_device_sensor.p_ctrl);
        if (SSP_SUCCESS != ret) {
            continue;
        }

        break;
    }

    if (ret != SSP_SUCCESS) {
        return ret;
    }

    *new_data = (byte & (1 << RAW_DATA_RDY_INT)) != 0;

    return ret;
}

#define int16_HL(X, Y) ((int16_t)(((uint16_t)(X) << 8) | ((uint8_t)(Y))))

ssp_err_t read_data_MPU6500(const sf_i2c_instance_t *device, double *data) {
    ssp_err_t ret = SSP_SUCCESS;
    uint8_t raw_data[6];

    for (size_t i = 0; i < I2C_RETRIES; i++) {
        device->p_api->lockWait(g_sf_i2c_device_sensor.p_ctrl, TX_WAIT_FOREVER);
        ret = i2c_read_register_n(device, ACCEL_XOUT_H, raw_data, 6, I2C_TIMEOUT);
        device->p_api->unlock(g_sf_i2c_device_sensor.p_ctrl);
        if (SSP_SUCCESS != ret) {
            continue;
        }

        break;
    }

    if (ret != SSP_SUCCESS) {
        return ret;
    }

    data[0] = scale_lsb_2g * int16_HL(raw_data[0], raw_data[1]);
    data[1] = scale_lsb_2g * int16_HL(raw_data[2], raw_data[3]);
    data[2] = scale_lsb_2g * int16_HL(raw_data[4], raw_data[5]);

    return ret;
}
