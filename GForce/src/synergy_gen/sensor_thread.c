/* generated thread source file - do not edit */
#include "sensor_thread.h"

TX_THREAD sensor_thread;
void sensor_thread_create(void);
static void sensor_thread_func(ULONG thread_input);
static uint8_t sensor_thread_stack[1024] BSP_PLACE_IN_SECTION_V2(".stack.sensor_thread") BSP_ALIGN_VARIABLE_V2(BSP_STACK_ALIGNMENT);
void tx_startup_err_callback(void *p_instance, void *p_data);
void tx_startup_common_init(void);
#if (0) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_external_irq_sensor) && !defined(SSP_SUPPRESS_ISR_ICU15)
SSP_VECTOR_DEFINE(icu_irq_isr, ICU, IRQ15);
#endif
#endif
static icu_instance_ctrl_t g_external_irq_sensor_ctrl;
static const external_irq_cfg_t g_external_irq_sensor_cfg =
{ .channel = 15,
  .trigger = EXTERNAL_IRQ_TRIG_RISING,
  .filter_enable = true,
  .pclk_div = EXTERNAL_IRQ_PCLK_DIV_BY_64,
  .autostart = true,
  .p_callback = NULL,
  .p_context = &g_external_irq_sensor,
  .p_extend = NULL,
  .irq_ipl = (0), };
/* Instance structure to use this module. */
const external_irq_instance_t g_external_irq_sensor =
{ .p_ctrl = &g_external_irq_sensor_ctrl, .p_cfg = &g_external_irq_sensor_cfg, .p_api = &g_external_irq_on_icu };
sf_external_irq_instance_ctrl_t g_sf_external_irq_sensor_ctrl;
const sf_external_irq_cfg_t g_sf_external_irq_sensor_cfg =
{ .event = SF_EXTERNAL_IRQ_EVENT_SEMAPHORE_PUT, .p_lower_lvl_irq = &g_external_irq_sensor, };
/* Instance structure to use this module. */
const sf_external_irq_instance_t g_sf_external_irq_sensor =
{ .p_ctrl = &g_sf_external_irq_sensor_ctrl, .p_cfg = &g_sf_external_irq_sensor_cfg, .p_api =
          &g_sf_external_irq_on_sf_external_irq };
/** Get driver cfg from bus and use all same settings except slave address and addressing mode. */
const i2c_cfg_t g_sf_i2c_device_sensor_i2c_cfg =
{ .channel = g_sf_i2c_bus_CHANNEL,
  .rate = g_sf_i2c_bus_RATE,
  .slave = 0x68,
  .addr_mode = I2C_ADDR_MODE_7BIT,
  .sda_delay = g_sf_i2c_bus_SDA_DELAY,
  .p_transfer_tx = g_sf_i2c_bus_P_TRANSFER_TX,
  .p_transfer_rx = g_sf_i2c_bus_P_TRANSFER_RX,
  .p_callback = g_sf_i2c_bus_P_CALLBACK,
  .p_context = g_sf_i2c_bus_P_CONTEXT,
  .rxi_ipl = g_sf_i2c_bus_RXI_IPL,
  .txi_ipl = g_sf_i2c_bus_TXI_IPL,
  .tei_ipl = g_sf_i2c_bus_TEI_IPL,
  .eri_ipl = g_sf_i2c_bus_ERI_IPL,
  .p_extend = g_sf_i2c_bus_P_EXTEND, };

sf_i2c_instance_ctrl_t g_sf_i2c_device_sensor_ctrl =
{ .p_lower_lvl_ctrl = &g_i2c_ctrl, };
const sf_i2c_cfg_t g_sf_i2c_device_sensor_cfg =
{ .p_bus = (sf_i2c_bus_t*) &g_sf_i2c_bus, .p_lower_lvl_cfg = &g_sf_i2c_device_sensor_i2c_cfg, };
/* Instance structure to use this module. */
const sf_i2c_instance_t g_sf_i2c_device_sensor =
{ .p_ctrl = &g_sf_i2c_device_sensor_ctrl, .p_cfg = &g_sf_i2c_device_sensor_cfg, .p_api = &g_sf_i2c_on_sf_i2c };
TX_SEMAPHORE g_data_semaphore;
TX_MUTEX g_data_mutex;
TX_SEMAPHORE g_unit_semaphore;
extern bool g_ssp_common_initialized;
extern uint32_t g_ssp_common_thread_count;
extern TX_SEMAPHORE g_ssp_common_initialized_semaphore;

void sensor_thread_create(void)
{
    /* Increment count so we will know the number of ISDE created threads. */
    g_ssp_common_thread_count++;

    /* Initialize each kernel object. */
    UINT err_g_data_semaphore;
    err_g_data_semaphore = tx_semaphore_create (&g_data_semaphore, (CHAR*) "Data Semaphore", 0);
    if (TX_SUCCESS != err_g_data_semaphore)
    {
        tx_startup_err_callback (&g_data_semaphore, 0);
    }
    UINT err_g_data_mutex;
    err_g_data_mutex = tx_mutex_create (&g_data_mutex, (CHAR*) "Data Mutex", TX_NO_INHERIT);
    if (TX_SUCCESS != err_g_data_mutex)
    {
        tx_startup_err_callback (&g_data_mutex, 0);
    }
    UINT err_g_unit_semaphore;
    err_g_unit_semaphore = tx_semaphore_create (&g_unit_semaphore, (CHAR*) "Unit Semaphore", 0);
    if (TX_SUCCESS != err_g_unit_semaphore)
    {
        tx_startup_err_callback (&g_unit_semaphore, 0);
    }

    UINT err;
    err = tx_thread_create (&sensor_thread, (CHAR*) "Sensor Thread", sensor_thread_func, (ULONG) NULL,
                            &sensor_thread_stack, 1024, 1, 1, 1, TX_AUTO_START);
    if (TX_SUCCESS != err)
    {
        tx_startup_err_callback (&sensor_thread, 0);
    }
}

static void sensor_thread_func(ULONG thread_input)
{
    /* Not currently using thread_input. */
    SSP_PARAMETER_NOT_USED (thread_input);

    /* Initialize common components */
    tx_startup_common_init ();

    /* Initialize each module instance. */

    /* Enter user code for this thread. */
    sensor_thread_entry ();
}
