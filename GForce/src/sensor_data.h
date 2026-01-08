/*
 * sensor_data.h
 *
 *  Created on: 3 ian. 2026
 *      Author: alexa
 */

#ifndef SENSOR_DATA_H_
#define SENSOR_DATA_H_

#define UNIT_M 0
#define UNIT_G 1

#define LEVEL_ZERO 0
#define LEVEL_LOW 1
#define LEVEL_NORMAL 2
#define LEVEL_HIGH 3
#define LEVEL_MAX 4

TX_SEMAPHORE* get_data_semaphore();
TX_SEMAPHORE* get_unit_semaphore();
TX_SEMAPHORE* get_time_semaphore();
TX_MUTEX* get_data_mutex();

typedef struct {
    uint8_t unit;
    uint8_t level;
    double accel;
    struct {
        double x, y, z;
    } axis;
} sensor_data_t;

#endif /* SENSOR_DATA_H_ */
