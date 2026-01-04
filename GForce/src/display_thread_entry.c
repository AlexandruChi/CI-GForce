#include <display_thread.h>
#include "bsp_api.h"
#include "gx_api.h"
#include "hardware/lcd.h"
#include "gui/GForceGUI_specifications.h"
#include "gui/GForceGUI_resources.h"
#include "sensor_data.h"
#include <stdio.h>

#define GX_EVENT_DATA (GX_FIRST_USER_EVENT + 1)

extern GX_CONST GX_STUDIO_WIDGET *GForceGUI_widget_table[];

INT encode_value(double value, uint8_t unit);

#define STATE_WAIT 0
#define STATE_READ 1
#define STATE_ENCODE 2
#define STATE_DISPLAY 3
#define STATE_ERROR ((uint8_t)-1)

extern sensor_data_t sensor_data;
sensor_data_t local_sensor_data;

void display_thread_entry(void) {
    /* Initialises GUIX. */
    if (TX_SUCCESS != gx_system_initialize()) {
        while(1);
    }

    /* Initialises GUIX drivers. */
    if (SSP_SUCCESS != g_sf_el_gx.p_api->open (g_sf_el_gx.p_ctrl, g_sf_el_gx.p_cfg)) {
        while(1);
    }

    GX_WINDOW_ROOT * p_window_root;

    gx_studio_display_configure(
            DISPLAY_1,
            g_sf_el_gx.p_api->setup,
            LANGUAGE_ENGLISH,
            DISPLAY_1_THEME_1,
            &p_window_root
    );

    if (SSP_SUCCESS != g_sf_el_gx.p_api->canvasInit(g_sf_el_gx.p_ctrl, p_window_root)) {
        while(1);
    }

    GX_CONST GX_STUDIO_WIDGET ** pp_studio_widget = &GForceGUI_widget_table[0];

    GX_WIDGET * p_screen = NULL;

    while (GX_NULL != *pp_studio_widget) {
        if (0 == strcmp("window1", (char*)(*pp_studio_widget)->widget_name)) {
            gx_studio_named_widget_create((*pp_studio_widget)->widget_name, (GX_WIDGET *)p_window_root, &p_screen);
        } else {
            gx_studio_named_widget_create((*pp_studio_widget)->widget_name, GX_NULL, GX_NULL);
        }

        pp_studio_widget++;
    }

    /* Shows the root window to make it and patients screen visible. */
    if (TX_SUCCESS != gx_widget_show(p_window_root)) {
        while(1);
    }

    /* Lets GUIX run. */
    if (TX_SUCCESS != gx_system_start()) {
        while(1);
    }

    /* Open the SPI driver to initialise the LCD */
    if (g_spi_lcdc.p_api->open(g_spi_lcdc.p_ctrl, (spi_cfg_t *)g_spi_lcdc.p_cfg)) {
        while(1);
    }

    ILI9341V_Init();

    struct {
        INT l, t, x, y, z;
    } display_data = {0, 0, 0, 0, 0};

    GX_EVENT g_gx_event;

    g_gx_event.gx_event_type = GX_EVENT_DATA;
    g_gx_event.gx_event_sender = GX_ID_NONE;
    g_gx_event.gx_event_target = p_screen;
    g_gx_event.gx_event_display_handle = GX_NULL;
    g_gx_event.gx_event_payload.gx_event_longdata = (LONG)&display_data;

    uint8_t state = STATE_WAIT;

    while (1) {
        switch (state) {
            case STATE_WAIT:
                tx_semaphore_get(get_data_semaphore(), TX_WAIT_FOREVER);
                state = STATE_READ;
                break;

            case STATE_READ:
                tx_mutex_get(get_data_mutex(), TX_WAIT_FOREVER);
                memcpy(&local_sensor_data, &sensor_data, sizeof(sensor_data_t));
                tx_mutex_put(get_data_mutex());
                state = STATE_ENCODE;
                break;

            case STATE_ENCODE:
                tx_mutex_get(&g_display_mutex, TX_WAIT_FOREVER);

                display_data.l = local_sensor_data.level;
                display_data.t = encode_value(local_sensor_data.accel, local_sensor_data.unit);
                display_data.x = encode_value(local_sensor_data.axis.x, local_sensor_data.unit);
                display_data.y = encode_value(local_sensor_data.axis.y, local_sensor_data.unit);
                display_data.z = encode_value(local_sensor_data.axis.z, local_sensor_data.unit);

                tx_mutex_put(&g_display_mutex);
                state = STATE_DISPLAY;
                break;

            case STATE_DISPLAY:
                gx_system_event_send(&g_gx_event);
                state = STATE_WAIT;
                break;

            case STATE_ERROR:
                while(1);
                break;

            default:
                state = STATE_WAIT;
                break;
        }
    }
}

void touch_panel_v2_callback(sf_touchpanel_v2_callback_args_t *p_args) {
    sf_touch_panel_v2_payload_t const * const p_payload = &p_args->payload;

    if (p_payload->event_type == SF_TOUCH_PANEL_V2_EVENT_DOWN) {
        tx_semaphore_ceiling_put(get_unit_semaphore(), 1);
    }
}

void g_lcd_spi_callback(spi_callback_args_t * p_args) {
    (void)p_args;
    tx_semaphore_ceiling_put(&g_display_semaphore_lcdc, 1);
}

static void update_prompt_value(GX_WIDGET *p_widget, GX_RESOURCE_ID id, INT value) {
    GX_NUMERIC_PROMPT *p = NULL;
    if (TX_SUCCESS == gx_widget_find(p_widget, (USHORT)id, GX_SEARCH_DEPTH_INFINITE, &p)) {
        gx_numeric_prompt_value_set(p, value);
    }
}

static void update_normal_fill(GX_WIDGET *p_widget, GX_RESOURCE_ID id, GX_RESOURCE_ID level) {
    GX_WIDGET *p = NULL;
    if (TX_SUCCESS == gx_widget_find(p_widget, (USHORT)id, GX_SEARCH_DEPTH_INFINITE, &p)) {
        gx_widget_fill_color_set(p, level, p->gx_widget_selected_fill_color);
    }
}

INT encode_value(double value, uint8_t unit) {
    INT ret = unit & 0b11;

    if (value < 0) {
        value = -value;
        ret |= 0b100;
    }

    value *= 100;

    return ((INT)value) << 3 | ret;
}

UINT window1_handler(GX_WINDOW *window, GX_EVENT *event_ptr) {
    UINT status = GX_SUCCESS;

    static const INT nr_levels = 5;
    static GX_RESOURCE_ID undef_level = GX_COLOR_ID_G_UNDEF;
    static GX_RESOURCE_ID levels[5] = {
        GX_COLOR_ID_G_ZERO,
        GX_COLOR_ID_G_LOW,
        GX_COLOR_ID_G_NORMAL,
        GX_COLOR_ID_G_HIGH,
        GX_COLOR_ID_G_MAX
    };

    switch (event_ptr->gx_event_type) {
        case GX_EVENT_DATA:
        {
            GX_RESOURCE_ID level;
            INT l, t, x, y, z;

            tx_mutex_get(&g_display_mutex, TX_WAIT_FOREVER);

            l = ((INT*)event_ptr->gx_event_payload.gx_event_longdata)[0];
            t = ((INT*)event_ptr->gx_event_payload.gx_event_longdata)[1];
            x = ((INT*)event_ptr->gx_event_payload.gx_event_longdata)[2];
            y = ((INT*)event_ptr->gx_event_payload.gx_event_longdata)[3];
            z = ((INT*)event_ptr->gx_event_payload.gx_event_longdata)[4];

            tx_mutex_put(&g_display_mutex);

            if (l >= nr_levels) {
                level = undef_level;
            } else {
                level = levels[l];
            }

            update_normal_fill((GX_WIDGET*)window, ID_AXIS_WINDOW, level);
            update_prompt_value((GX_WIDGET*)window, ID_TOTAL,  t);
            update_prompt_value((GX_WIDGET*)window, ID_AXIS_X, x);
            update_prompt_value((GX_WIDGET*)window, ID_AXIS_Y, y);
            update_prompt_value((GX_WIDGET*)window, ID_AXIS_Z, z);
        }

            break;

        case GX_EVENT_SHOW:
            status = gx_window_event_process(window, event_ptr);
            update_normal_fill((GX_WIDGET*)window, ID_AXIS_WINDOW, undef_level);
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

#define UNIT_M_STR "m/s\xC2\xB2"
#define UNIT_G_STR "g   "
#define UNIT_NONE_STR "    "

void format_gforce(GX_NUMERIC_PROMPT *p_prompt, INT value) {
    char *prefix = "";
    switch (p_prompt->gx_widget_id) {
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

    uint8_t unit = value & 0b11;
    value >>= 2;

    char *unit_str = UNIT_NONE_STR;
    switch (unit) {
        case UNIT_M:
            unit_str = UNIT_M_STR;
            break;

        case UNIT_G:
            unit_str = UNIT_G_STR;
            break;

        default:
            unit_str = UNIT_NONE_STR;
            break;
    }

    char *sign = "";
    if (p_prompt->gx_widget_id != ID_TOTAL) {
        sign = value % 2 ? "-" : " ";
    }

    value >>= 1;

    INT int_part = value / 100;
    INT dec_part = value % 100;

    snprintf(
            p_prompt->gx_numeric_prompt_buffer,
            GX_NUMERIC_PROMPT_BUFFER_SIZE,
            "%s%s%2d.%02d %s",
            prefix,
            sign,
            int_part,
            dec_part,
            unit_str
    );
}
