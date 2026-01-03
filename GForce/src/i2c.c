/*
 * i2c.c
 *
 *  Created on: 1 ian. 2026
 *      Author: alexa
 */

#include "i2c.h"

ssp_err_t i2c_write(const sf_i2c_instance_t *device, uint8_t val, uint32_t timeout)
{
    return device->p_api->write(device->p_ctrl, &val, 1, 0, timeout);
}

ssp_err_t i2c_read(const sf_i2c_instance_t *device, uint8_t *val, uint32_t timeout)
{
    return device->p_api->read(device->p_ctrl, val, 1, 0, timeout);
}

ssp_err_t i2c_write_n(const sf_i2c_instance_t *device, uint8_t *val, uint32_t size, uint32_t timeout)
{
    return device->p_api->write(device->p_ctrl, val, size, 0, timeout);
}

ssp_err_t i2c_read_n(const sf_i2c_instance_t *device, uint8_t *val, uint32_t size, uint32_t timeout)
{
    return device->p_api->read(device->p_ctrl, val, size, 0, timeout);
}

ssp_err_t i2c_write_register(const sf_i2c_instance_t *device, uint8_t reg, uint8_t val, uint32_t timeout)
{
    uint8_t buffer[2] = {reg, val};
    return device->p_api->write(device->p_ctrl, buffer, 2, 0, timeout);
}

ssp_err_t i2c_read_register(const sf_i2c_instance_t *device, uint8_t reg, uint8_t *val, uint32_t timeout)
{
    ssp_err_t ret;

    if
    (
            SSP_SUCCESS != (ret = device->p_api->write(device->p_ctrl, &reg, 1, 1, timeout)) ||
            SSP_SUCCESS != (ret = device->p_api->read(device->p_ctrl, val, 1, 0, timeout))
    )
    {
    }

    return ret;
}

ssp_err_t i2c_write_register_n(const sf_i2c_instance_t *device, uint8_t reg, uint8_t *val, uint32_t size, uint32_t timeout)
{
    ssp_err_t ret;

    if
    (
            SSP_SUCCESS != (ret = device->p_api->write(device->p_ctrl, &reg, 1, 1, timeout)) ||
            SSP_SUCCESS != (ret = device->p_api->write(device->p_ctrl, val, size, 0, timeout))
    )
    {
    }

    return ret;
}

ssp_err_t i2c_read_register_n(const sf_i2c_instance_t *device, uint8_t reg, uint8_t *val, uint32_t size, uint32_t timeout)
{
    ssp_err_t ret;

    if
    (
            SSP_SUCCESS != (ret = device->p_api->write(device->p_ctrl, &reg, 1, 1, timeout)) ||
            SSP_SUCCESS != (ret = device->p_api->read(device->p_ctrl, val, size, 0, timeout))
    )
    {
    }

    return ret;
}

ssp_err_t i2c_write_register_bit(const sf_i2c_instance_t *device, uint8_t reg, uint8_t bit, uint8_t val, uint32_t timeout)
{
    ssp_err_t ret;
    uint8_t byte;

    if (SSP_SUCCESS == (ret = i2c_read_register(device, reg, &byte, timeout)))
    {
        if (val & 0b1)
        {
            byte |= (uint8_t)(0b1 << bit);
        }
        else
        {
            byte &= (uint8_t)(~(0b1 << bit));
        }

        return i2c_write_register(device, reg, byte, timeout);
    }

    return ret;
}

ssp_err_t i2c_read_register_bit(const sf_i2c_instance_t *device, uint8_t reg, uint8_t bit, uint8_t *val, uint32_t timeout)
{
    ssp_err_t ret;
    uint8_t byte;

    if (SSP_SUCCESS == (ret = i2c_read_register(device, reg, &byte, timeout)))
    {
        *val = (*val >> bit) &0b1;
    }

    return ret;
}

ssp_err_t i2c_write_register_bit_n(const sf_i2c_instance_t *device, uint8_t reg, uint8_t bit, uint8_t val, uint8_t size, uint32_t timeout)
{
    ssp_err_t ret;
    uint8_t byte;

    if (SSP_SUCCESS == (ret = i2c_read_register(device, reg, &byte, timeout)))
    {
        for (uint8_t i = 0; i < size; i++)
        {
            if (val & 0b1)
            {
                byte |= (uint8_t)(0b1 << (bit + i));
            }
            else
            {
                byte &= (uint8_t)(~(0b1 << (bit + i)));
            }

            val >>= 1;
        }

        return i2c_write_register(device, reg, byte, timeout);
    }

    return ret;
}

ssp_err_t i2c_read_register_bit_n(const sf_i2c_instance_t *device, uint8_t reg, uint8_t bit, uint8_t *val, uint8_t size, uint32_t timeout)
{
    ssp_err_t ret;
    uint8_t byte;

    if (SSP_SUCCESS == (ret = i2c_read_register(device, reg, &byte, timeout)))
    {
        *val = (uint8_t)((*val >> bit) & ((1 << size) - 1));
    }

    return ret;
}
