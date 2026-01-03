/*
 * i2c.h
 *
 *  Created on: 1 ian. 2026
 *      Author: alexa
 */

#ifndef I2C_H_
#define I2C_H_

#include "sf_i2c_api.h"

ssp_err_t i2c_write(const sf_i2c_instance_t *device, uint8_t val, uint32_t timeout);
ssp_err_t i2c_read(const sf_i2c_instance_t *device, uint8_t *val, uint32_t timeout);

ssp_err_t i2c_write_n(const sf_i2c_instance_t *device, uint8_t *val, uint32_t size, uint32_t timeout);
ssp_err_t i2c_read_n(const sf_i2c_instance_t *device, uint8_t *val, uint32_t size, uint32_t timeout);

ssp_err_t i2c_write_register(const sf_i2c_instance_t *device, uint8_t reg, uint8_t val, uint32_t timeout);
ssp_err_t i2c_read_register(const sf_i2c_instance_t *device, uint8_t reg, uint8_t *val, uint32_t timeout);

ssp_err_t i2c_write_register_n(const sf_i2c_instance_t *device, uint8_t reg, uint8_t *val, uint32_t size, uint32_t timeout);
ssp_err_t i2c_read_register_n(const sf_i2c_instance_t *device, uint8_t reg, uint8_t *val, uint32_t size, uint32_t timeout);

ssp_err_t i2c_write_register_bit(const sf_i2c_instance_t *device, uint8_t reg, uint8_t bit, uint8_t val, uint32_t timeout);
ssp_err_t i2c_read_register_bit(const sf_i2c_instance_t *device, uint8_t reg, uint8_t bit, uint8_t *val, uint32_t timeout);

ssp_err_t i2c_write_register_bit_n(const sf_i2c_instance_t *device, uint8_t reg, uint8_t bit, uint8_t val, uint8_t size, uint32_t timeout);
ssp_err_t i2c_read_register_bit_n(const sf_i2c_instance_t *device, uint8_t reg, uint8_t bit, uint8_t *val, uint8_t size, uint32_t timeout);

#endif /* I2C_H_ */
