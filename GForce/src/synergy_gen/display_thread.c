/* generated thread source file - do not edit */
#include "display_thread.h"

TX_THREAD display_thread;
void display_thread_create(void);
static void display_thread_func(ULONG thread_input);
static uint8_t display_thread_stack[2048] BSP_PLACE_IN_SECTION_V2(".stack.display_thread") BSP_ALIGN_VARIABLE_V2(BSP_STACK_ALIGNMENT);
void tx_startup_err_callback(void *p_instance, void *p_data);
void tx_startup_common_init(void);
#if !defined(SSP_SUPPRESS_ISR_g_spi_lcdc) && !defined(SSP_SUPPRESS_ISR_SCI0)
SSP_VECTOR_DEFINE_CHAN(sci_spi_rxi_isr, SCI, RXI, 0);
#endif
#if !defined(SSP_SUPPRESS_ISR_g_spi_lcdc) && !defined(SSP_SUPPRESS_ISR_SCI0)
SSP_VECTOR_DEFINE_CHAN(sci_spi_txi_isr, SCI, TXI, 0);
#endif
#if !defined(SSP_SUPPRESS_ISR_g_spi_lcdc) && !defined(SSP_SUPPRESS_ISR_SCI0)
SSP_VECTOR_DEFINE_CHAN(sci_spi_tei_isr, SCI, TEI, 0);
#endif
#if !defined(SSP_SUPPRESS_ISR_g_spi_lcdc) && !defined(SSP_SUPPRESS_ISR_SCI0)
SSP_VECTOR_DEFINE_CHAN(sci_spi_eri_isr, SCI, ERI, 0);
#endif
sci_spi_instance_ctrl_t g_spi_lcdc_ctrl;

/** SPI extended configuration */
const sci_spi_extended_cfg g_spi_lcdc_cfg_extend =
{ .bitrate_modulation = true };

const spi_cfg_t g_spi_lcdc_cfg =
{ .channel = 0, .operating_mode = SPI_MODE_MASTER, .clk_phase = SPI_CLK_PHASE_EDGE_EVEN, .clk_polarity =
          SPI_CLK_POLARITY_HIGH,
  .mode_fault = SPI_MODE_FAULT_ERROR_DISABLE, .bit_order = SPI_BIT_ORDER_MSB_FIRST, .bitrate = 100000,
#define SYNERGY_NOT_DEFINED (1)             
#if (SYNERGY_NOT_DEFINED == SYNERGY_NOT_DEFINED)
  .p_transfer_tx = NULL,
#else
                .p_transfer_tx       = &SYNERGY_NOT_DEFINED,
#endif
#if (SYNERGY_NOT_DEFINED == SYNERGY_NOT_DEFINED)
  .p_transfer_rx = NULL,
#else
                .p_transfer_rx       = &SYNERGY_NOT_DEFINED,
#endif
#undef SYNERGY_NOT_DEFINED	
  .p_callback = g_lcd_spi_callback,
  .p_context = (void*) &g_spi_lcdc, .rxi_ipl = (3), .txi_ipl = (3), .tei_ipl = (3), .eri_ipl = (3), .p_extend =
          &g_spi_lcdc_cfg_extend, };
/* Instance structure to use this module. */
const spi_instance_t g_spi_lcdc =
{ .p_ctrl = &g_spi_lcdc_ctrl, .p_cfg = &g_spi_lcdc_cfg, .p_api = &g_spi_on_sci };
#if (3) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_external_irq0) && !defined(SSP_SUPPRESS_ISR_ICU9)
SSP_VECTOR_DEFINE( icu_irq_isr, ICU, IRQ9);
#endif
#endif
static icu_instance_ctrl_t g_external_irq0_ctrl;
static const external_irq_cfg_t g_external_irq0_cfg =
{ .channel = 9,
  .trigger = EXTERNAL_IRQ_TRIG_FALLING,
  .filter_enable = true,
  .pclk_div = EXTERNAL_IRQ_PCLK_DIV_BY_64,
  .autostart = true,
  .p_callback = NULL,
  .p_context = &g_external_irq0,
  .p_extend = NULL,
  .irq_ipl = (3), };
/* Instance structure to use this module. */
const external_irq_instance_t g_external_irq0 =
{ .p_ctrl = &g_external_irq0_ctrl, .p_cfg = &g_external_irq0_cfg, .p_api = &g_external_irq_on_icu };
sf_external_irq_instance_ctrl_t g_sf_external_irq0_ctrl;
const sf_external_irq_cfg_t g_sf_external_irq0_cfg =
{ .event = SF_EXTERNAL_IRQ_EVENT_SEMAPHORE_PUT, .p_lower_lvl_irq = &g_external_irq0, };
/* Instance structure to use this module. */
const sf_external_irq_instance_t g_sf_external_irq0 =
{ .p_ctrl = &g_sf_external_irq0_ctrl, .p_cfg = &g_sf_external_irq0_cfg, .p_api = &g_sf_external_irq_on_sf_external_irq };
/** Get driver cfg from bus and use all same settings except slave address and addressing mode. */
const i2c_cfg_t g_sf_i2c_device0_i2c_cfg =
{ .channel = g_sf_i2c_bus0_CHANNEL,
  .rate = g_sf_i2c_bus0_RATE,
  .slave = 0x48,
  .addr_mode = I2C_ADDR_MODE_7BIT,
  .sda_delay = g_sf_i2c_bus0_SDA_DELAY,
  .p_transfer_tx = g_sf_i2c_bus0_P_TRANSFER_TX,
  .p_transfer_rx = g_sf_i2c_bus0_P_TRANSFER_RX,
  .p_callback = g_sf_i2c_bus0_P_CALLBACK,
  .p_context = g_sf_i2c_bus0_P_CONTEXT,
  .rxi_ipl = g_sf_i2c_bus0_RXI_IPL,
  .txi_ipl = g_sf_i2c_bus0_TXI_IPL,
  .tei_ipl = g_sf_i2c_bus0_TEI_IPL,
  .eri_ipl = g_sf_i2c_bus0_ERI_IPL,
  .p_extend = g_sf_i2c_bus0_P_EXTEND, };

sf_i2c_instance_ctrl_t g_sf_i2c_device0_ctrl =
{ .p_lower_lvl_ctrl = &g_i2c_ctrl, };
const sf_i2c_cfg_t g_sf_i2c_device0_cfg =
{ .p_bus = (sf_i2c_bus_t*) &g_sf_i2c_bus0, .p_lower_lvl_cfg = &g_sf_i2c_device0_i2c_cfg, };
/* Instance structure to use this module. */
const sf_i2c_instance_t g_sf_i2c_device0 =
{ .p_ctrl = &g_sf_i2c_device0_ctrl, .p_cfg = &g_sf_i2c_device0_cfg, .p_api = &g_sf_i2c_on_sf_i2c };
sf_touch_panel_chip_sx8654_instance_ctrl_t g_touch_panel_chip_sx8654_ctrl;
const sf_touch_panel_chip_on_sx8654_cfg_t g_touch_panel_chip_sx8654_cfg_extend =
{ .pin = IOPORT_PORT_06_PIN_09,
  .p_lower_lvl_framewrk = &g_sf_i2c_device0,
  .p_lower_lvl_irq = &g_sf_external_irq0,
  .hsize_pixels = 240,
  .vsize_pixels = 320 };
const sf_touch_panel_chip_cfg_t g_touch_panel_chip_sx8654_cfg =
{ .p_extend = &g_touch_panel_chip_sx8654_cfg_extend };
const sf_touch_panel_chip_instance_t g_touch_panel_chip_sx8654 =
{ .p_ctrl = &g_touch_panel_chip_sx8654_ctrl, .p_cfg = &g_touch_panel_chip_sx8654_cfg, .p_api =
          &g_sf_touch_panel_chip_sx8654 };
#if defined(__ICCARM__)
            #define g_sf_touch_panel_err_callback_WEAK_ATTRIBUTE
            #pragma weak g_sf_touch_panel_err_callback  = g_sf_touch_panel_err_callback_internal
            #elif defined(__GNUC__)
            #define g_sf_touch_panel_err_callback_WEAK_ATTRIBUTE   __attribute__ ((weak, alias("g_sf_touch_panel_err_callback_internal")))
            #endif
void g_sf_touch_panel_err_callback(void *p_instance, void *p_data)
g_sf_touch_panel_err_callback_WEAK_ATTRIBUTE;
sf_touch_panel_v2_instance_ctrl_t g_sf_touch_panel_ctrl;
const sf_touch_panel_v2_extend_cfg_t g_sf_touch_panel_cfg_extend =
{ .p_chip = &g_touch_panel_chip_sx8654 };
const sf_touch_panel_v2_cfg_t g_sf_touch_panel_cfg =
{ .hsize_pixels = 240,
  .vsize_pixels = 320,
  .priority = 3,
  .update_hz = 10,
  .p_extend = &g_sf_touch_panel_cfg_extend,
  .rotation_angle = 0,
  .p_callback = touch_panel_v2_callback,
  .p_context = &g_sf_touch_panel };

/* Instance structure to use this module. */
const sf_touch_panel_v2_instance_t g_sf_touch_panel =
{ .p_ctrl = &g_sf_touch_panel_ctrl, .p_cfg = &g_sf_touch_panel_cfg, .p_api = &g_sf_touch_panel_v2_on_sf_touch_panel_v2 };
/*******************************************************************************************************************//**
 * @brief      Initialization function that the user can choose to have called automatically during thread entry.
 *             The user can call this function at a later time if desired using the prototype below.

 *             - void g_sf_touch_panel_err_callback(void * p_instance, void * p_data)
 *
 * @param[in]  p_instance arguments used to identify which instance caused the error and p_data Callback arguments
 used to identify what error caused the callback.
 **********************************************************************************************************************/
void g_sf_touch_panel_err_callback_internal(void *p_instance, void *p_data);
void g_sf_touch_panel_err_callback_internal(void *p_instance, void *p_data)
{
    /** Suppress compiler warning for not using parameters. */
    SSP_PARAMETER_NOT_USED (p_instance);
    SSP_PARAMETER_NOT_USED (p_data);
    /** An error has occurred. Please check function arguments for more information. */
    BSP_CFG_HANDLE_UNRECOVERABLE_ERROR (0);
}
/*******************************************************************************************************************//**
 * @brief     This is sf touch panel initialization function. User Can call this function in the application 
 if required with the below mentioned prototype.
 *            - void sf_touch_panel_v2_init(void)
 **********************************************************************************************************************/
void sf_touch_panel_v2_init(void)
{
    ssp_err_t ssp_err_g_sf_touch_panel;
    ssp_err_g_sf_touch_panel = g_sf_touch_panel.p_api->open (g_sf_touch_panel.p_ctrl, g_sf_touch_panel.p_cfg);
    if (SSP_SUCCESS != ssp_err_g_sf_touch_panel)
    {
        g_sf_touch_panel_err_callback ((void*) &g_sf_touch_panel, &ssp_err_g_sf_touch_panel);
    }
    if (1)
    {
        ssp_err_g_sf_touch_panel = g_sf_touch_panel.p_api->start (g_sf_touch_panel.p_ctrl);
        if (SSP_SUCCESS != ssp_err_g_sf_touch_panel)
        {
            g_sf_touch_panel_err_callback ((void*) &g_sf_touch_panel, &ssp_err_g_sf_touch_panel);
        }
    }
}
TX_SEMAPHORE g_display_semaphore_lcdc;
TX_EVENT_FLAGS_GROUP g_touch_event_flags;
TX_SEMAPHORE g_tap_semaphore;
extern bool g_ssp_common_initialized;
extern uint32_t g_ssp_common_thread_count;
extern TX_SEMAPHORE g_ssp_common_initialized_semaphore;

void display_thread_create(void)
{
    /* Increment count so we will know the number of ISDE created threads. */
    g_ssp_common_thread_count++;

    /* Initialize each kernel object. */
    UINT err_g_display_semaphore_lcdc;
    err_g_display_semaphore_lcdc = tx_semaphore_create (&g_display_semaphore_lcdc, (CHAR*) "Display Semaphore", 0);
    if (TX_SUCCESS != err_g_display_semaphore_lcdc)
    {
        tx_startup_err_callback (&g_display_semaphore_lcdc, 0);
    }
    UINT err_g_touch_event_flags;
    err_g_touch_event_flags = tx_event_flags_create (&g_touch_event_flags, (CHAR*) "New Event Flags");
    if (TX_SUCCESS != err_g_touch_event_flags)
    {
        tx_startup_err_callback (&g_touch_event_flags, 0);
    }
    UINT err_g_tap_semaphore;
    err_g_tap_semaphore = tx_semaphore_create (&g_tap_semaphore, (CHAR*) "Tap Semaphore", 0);
    if (TX_SUCCESS != err_g_tap_semaphore)
    {
        tx_startup_err_callback (&g_tap_semaphore, 0);
    }

    UINT err;
    err = tx_thread_create (&display_thread, (CHAR*) "Display Thread", display_thread_func, (ULONG) NULL,
                            &display_thread_stack, 2048, 6, 6, 10, TX_AUTO_START);
    if (TX_SUCCESS != err)
    {
        tx_startup_err_callback (&display_thread, 0);
    }
}

static void display_thread_func(ULONG thread_input)
{
    /* Not currently using thread_input. */
    SSP_PARAMETER_NOT_USED (thread_input);

    /* Initialize common components */
    tx_startup_common_init ();

    /* Initialize each module instance. */
    /** Call initialization function if user has selected to do so. */
#if (1)
    sf_touch_panel_v2_init ();
#endif

    /* Enter user code for this thread. */
    display_thread_entry ();
}
