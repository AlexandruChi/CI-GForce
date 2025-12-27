#include <GForceGUI_thread.h>
#include "../gui/GForceGUI_resources.h"
#include "../gui/GForceGUI_specifications.h"
#include <stdio.h>
#include "GForceGUI_events.h"

#define UNIT_G "g"
#define UNIT_M "m/s\xC2\xB2"

static char *unit_g = UNIT_G;
static char *unit_m = UNIT_M;

static char *unit = UNIT_G;

UINT window1_handler(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
    UINT result = gx_window_event_process(widget, event_ptr);

    if (event_ptr->gx_event_type == GX_EVENT_TOGGLE_UNIT)
    {
        if (unit == unit_g)
        {
            unit = unit_m;
        }
        else
        {
            unit = unit_g;
        }
    }
    else
    {
        gx_window_event_process(widget, event_ptr);
    }

    return result;
}

void format_gforce(GX_NUMERIC_PROMPT *p_prompt, INT value)
{
    char sign_char = ' ';
    if (value < 0)
    {
        sign_char = '-';
        value = -value;
    }

    INT int_part = value / 1000;
    INT dec_part = value % 1000;

    snprintf(p_prompt->gx_numeric_prompt_buffer,
             GX_NUMERIC_PROMPT_BUFFER_SIZE,
             "%c%2d.%03d %s",
             sign_char,
             int_part,
             dec_part,
             unit);
}
