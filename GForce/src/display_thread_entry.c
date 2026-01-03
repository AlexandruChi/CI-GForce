#include <display_thread.h>
#include "bsp_api.h"
#include "gx_api.h"
#include "hardware/lcd.h"
#include "gui/GForceGUI_specifications.h"
#include "gui/GForceGUI_resources.h"
#include <stdio.h>

#define GX_EVENT_DATA (GX_FIRST_USER_EVENT + 1)

extern GX_CONST GX_STUDIO_WIDGET *GForceGUI_widget_table[];

static void update_prompt_value(GX_WIDGET *p_widget, GX_RESOURCE_ID id, INT valu);

void display_thread_entry(void) {
    if
    (
            TX_SUCCESS != gx_system_initialize() ||                                     /* Initialises GUIX. */
            SSP_SUCCESS != g_sf_el_gx.p_api->open (g_sf_el_gx.p_ctrl, g_sf_el_gx.p_cfg) /* Initialises GUIX drivers. */
    )
    {
        while(1);
    }

    GX_WINDOW_ROOT * p_window_root;

    gx_studio_display_configure ( DISPLAY_1,
                                  g_sf_el_gx.p_api->setup,
                                  LANGUAGE_ENGLISH,
                                  DISPLAY_1_THEME_1,
                                  &p_window_root );

    if (SSP_SUCCESS != g_sf_el_gx.p_api->canvasInit(g_sf_el_gx.p_ctrl, p_window_root))
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

    if
    (
            TX_SUCCESS != gx_widget_show(p_window_root) ||                              /* Shows the root window to make it and patients screen visible. */
            TX_SUCCESS != gx_system_start() ||                                          /* Lets GUIX run. */
            g_spi_lcdc.p_api->open(g_spi_lcdc.p_ctrl, (spi_cfg_t *)g_spi_lcdc.p_cfg)    /* Open the SPI driver to initialise the LCD */
    )
    {
        while(1);
    }

    ILI9341V_Init();

    struct
    {
        INT t, x, y, z;
    }
    data = {0, 0, 0, 0};

    GX_EVENT g_gx_event;

    g_gx_event.gx_event_type = GX_EVENT_DATA;
    g_gx_event.gx_event_sender = GX_ID_NONE;
    g_gx_event.gx_event_target = p_screen;
    g_gx_event.gx_event_display_handle = GX_NULL;
    g_gx_event.gx_event_payload.gx_event_longdata = (LONG)&data;

    while (1)
    {
        tx_semaphore_get(&g_tap_semaphore, TX_WAIT_FOREVER);

        tx_mutex_get(&g_display_mutex, TX_WAIT_FOREVER);

        data.t++;
        data.x++;
        data.y++;
        data.z++;

        tx_mutex_put(&g_display_mutex);

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

static void update_prompt_value(GX_WIDGET *p_widget, GX_RESOURCE_ID id, INT value)
{
    GX_NUMERIC_PROMPT *p_prompt = NULL;
    if (TX_SUCCESS == gx_widget_find(p_widget, (USHORT)id, GX_SEARCH_DEPTH_INFINITE, &p_prompt))
    {
        gx_numeric_prompt_value_set(p_prompt, value);
    }
}

UINT window1_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    UINT status = GX_SUCCESS;

    switch (event_ptr->gx_event_type)
    {
        case GX_EVENT_DATA:
        {
            INT t, x, y, z;

            tx_mutex_get(&g_display_mutex, TX_WAIT_FOREVER);

            t = ((INT*)event_ptr->gx_event_payload.gx_event_longdata)[0];
            x = ((INT*)event_ptr->gx_event_payload.gx_event_longdata)[1];
            y = ((INT*)event_ptr->gx_event_payload.gx_event_longdata)[2];
            z = ((INT*)event_ptr->gx_event_payload.gx_event_longdata)[3];

            tx_mutex_put(&g_display_mutex);

            update_prompt_value((GX_WIDGET*)window, ID_TOTAL,  t);
            update_prompt_value((GX_WIDGET*)window, ID_AXIS_X, x);
            update_prompt_value((GX_WIDGET*)window, ID_AXIS_Y, y);
            update_prompt_value((GX_WIDGET*)window, ID_AXIS_Z, z);
        }

            break;

        case GX_EVENT_SHOW:
            status = gx_window_event_process(window, event_ptr);
            update_prompt_value((GX_WIDGET*)window, ID_TOTAL,  0);
            update_prompt_value((GX_WIDGET*)window, ID_AXIS_X, 0);
            update_prompt_value((GX_WIDGET*)window, ID_AXIS_Y, 0);
            update_prompt_value((GX_WIDGET*)window, ID_AXIS_Z, 0);
            break;

        default:
            status = gx_window_event_process(window, event_ptr);
            break;
    }

    return status;
}

#define UNIT_M "m/s\xC2\xB2"
#define UNIT_G "g   "

void format_gforce(GX_NUMERIC_PROMPT *p_prompt, INT value)
{
    char *prefix = "";
    switch (p_prompt->gx_widget_id)
    {
        case ID_AXIS_X:
            prefix = "X: ";
            break;

        case ID_AXIS_Y:
            prefix = "Y: ";
            break;

        case ID_AXIS_Z:
            prefix = "Z: ";
            break;

        default:
            break;
    }

    char *unit = value % 2 ? UNIT_G : UNIT_M;
    value >>= 1;

    char sign_char = value % 2 ? '-' : ' ';
    value >>= 1;

    INT int_part = value / 100;
    INT dec_part = value % 100;

    snprintf(p_prompt->gx_numeric_prompt_buffer,
             GX_NUMERIC_PROMPT_BUFFER_SIZE,
             "%s%c%2d.%02d %s",
             prefix,
             sign_char,
             int_part,
             dec_part,
             unit);
}
