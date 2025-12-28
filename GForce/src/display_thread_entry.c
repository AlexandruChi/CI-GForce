#include <display_thread.h>
#include "bsp_api.h"
#include "gx_api.h"
#include "hardware/lcd.h"
#include "GForceGUI/GForceGUI_events.h"
#include "gui/GForceGUI_specifications.h"
#include "gui/GForceGUI_resources.h"

static GX_WINDOW_ROOT * p_window_root;
extern GX_CONST GX_STUDIO_WIDGET *GForceGUI_widget_table[];

void display_thread_entry(void) {
    if(
            TX_SUCCESS != gx_system_initialize() ||                                     /* Initializes GUIX. */
            SSP_SUCCESS != g_sf_el_gx.p_api->open (g_sf_el_gx.p_ctrl, g_sf_el_gx.p_cfg) /* Initializes GUIX drivers. */
    ) {
        while(1);
    }

    gx_studio_display_configure ( DISPLAY_1,
                                  g_sf_el_gx.p_api->setup,
                                  LANGUAGE_ENGLISH,
                                  DISPLAY_1_THEME_1,
                                  &p_window_root );

    if(SSP_SUCCESS != g_sf_el_gx.p_api->canvasInit(g_sf_el_gx.p_ctrl, p_window_root))
    {
        while(1);
    }

    GX_CONST GX_STUDIO_WIDGET ** pp_studio_widget = &GForceGUI_widget_table[0];
    GX_WIDGET * p_screen = NULL;

    while (GX_NULL != *pp_studio_widget)
    {
        if (0 == strcmp("window1", (char*)(*pp_studio_widget)->widget_name))
        {
            gx_studio_named_widget_create((*pp_studio_widget)->widget_name, (GX_WIDGET *)p_window_root, &p_screen);
        } else {
            gx_studio_named_widget_create((*pp_studio_widget)->widget_name, GX_NULL, GX_NULL);
        }

        pp_studio_widget++;
    }

    // gx_widget_attach(p_window_root, p_screen);

    if(
            TX_SUCCESS != gx_widget_show(p_window_root) ||                              /* Shows the root window to make it and patients screen visible. */
            TX_SUCCESS != gx_system_start() ||                                          /* Lets GUIX run. */
            g_spi_lcdc.p_api->open(g_spi_lcdc.p_ctrl, (spi_cfg_t *)g_spi_lcdc.p_cfg)    /* Open the SPI driver to initialize the LCD */
    ) {
        while(1);
    }

    ILI9341V_Init();

    GX_EVENT g_gx_event;

    g_gx_event.gx_event_type = GX_EVENT_TOGGLE_UNIT;
    g_gx_event.gx_event_sender = GX_ID_NONE;
    g_gx_event.gx_event_target = p_screen;
    g_gx_event.gx_event_display_handle = GX_NULL;

    while (1)
    {
        tx_semaphore_get(&g_tap_semaphore, TX_WAIT_FOREVER);
        gx_system_event_send(&g_gx_event);
    }
}

void touch_panel_v2_callback(sf_touchpanel_v2_callback_args_t *p_args)
{
    sf_touch_panel_v2_payload_t const * const p_payload = &p_args->payload;

    if (p_payload->event_type == SF_TOUCH_PANEL_V2_EVENT_DOWN)
    {
        tx_semaphore_ceiling_put(&g_tap_semaphore, 1);
    }
}

void g_lcd_spi_callback(spi_callback_args_t * p_args)
{
    (void)p_args;
    tx_semaphore_ceiling_put(&g_display_semaphore_lcdc, 1);
}
