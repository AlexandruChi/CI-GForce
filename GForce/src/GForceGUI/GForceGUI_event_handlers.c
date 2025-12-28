#include "../gui/GForceGUI_resources.h"
#include "../gui/GForceGUI_specifications.h"
#include "GForceGUI_events.h"
#include <stdio.h>

static void update_prompt_value(GX_WIDGET *p_widget, GX_RESOURCE_ID id, INT valu);

UINT window1_handler(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
    static char current_unit = 0;

    static INT value_t = -123;
    static INT value_x = 5678;
    static INT value_y = 453;
    static INT value_z = -34;

    UINT result = gx_window_event_process(widget, event_ptr);

    switch (event_ptr->gx_event_type)
    {
        case GX_EVENT_TOGGLE_UNIT:
            current_unit = current_unit ? 0 : 1;

            /* fallthrough */
            /* no break */

        case GX_EVENT_SHOW:
            update_prompt_value(widget->gx_widget_parent, ID_TOTAL,  value_t * 2 + current_unit);
            update_prompt_value(widget->gx_widget_parent, ID_AXIS_X, value_x * 2 + current_unit);
            update_prompt_value(widget->gx_widget_parent, ID_AXIS_Y, value_y * 2 + current_unit);
            update_prompt_value(widget->gx_widget_parent, ID_AXIS_Z, value_z * 2 + current_unit);
            break;

        default:
            gx_window_event_process(widget, event_ptr);
            break;
    }

    return result;
}

static void update_prompt_value(GX_WIDGET *p_widget, GX_RESOURCE_ID id, INT value)
{
    GX_NUMERIC_PROMPT *p_prompt = NULL;
    if (TX_SUCCESS == gx_widget_find(p_widget, (USHORT)id, GX_SEARCH_DEPTH_INFINITE, &p_prompt)) {
        gx_numeric_prompt_value_set(p_prompt, value);
    }
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
    value /= 2;

    char sign_char = ' ';
    if (value < 0)
    {
        sign_char = '-';
        value = -value;
    }

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
