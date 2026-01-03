/* generated thread header file - do not edit */
#ifndef SENSOR_THREAD_H_
#define SENSOR_THREAD_H_
#include "bsp_api.h"
#include "tx_api.h"
#include "hal_data.h"
#ifdef __cplusplus
                extern "C" void sensor_thread_entry(void);
                #else
extern void sensor_thread_entry(void);
#endif
#include "r_icu.h"
#include "r_external_irq_api.h"
#include "sf_external_irq.h"
#include "sf_i2c.h"
#include "sf_i2c_api.h"
#ifdef __cplusplus
extern "C" {
#endif
/* External IRQ on ICU Instance. */
extern const external_irq_instance_t g_external_irq_sensor;
#ifndef NULL
void NULL(external_irq_callback_args_t *p_args);
#endif
/** SF External IRQ on SF External IRQ Instance. */
extern const sf_external_irq_instance_t g_sf_external_irq_sensor;
/* SF I2C on SF I2C Instance. */
extern const sf_i2c_instance_t g_sf_i2c_device_sensor;
extern TX_SEMAPHORE g_data_semaphore;
#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* SENSOR_THREAD_H_ */
