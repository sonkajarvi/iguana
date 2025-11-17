/**
 * Copyright (c) 2025, sonkajarvi
 *
 * Licensed under the BSD 2-Clause License.
 * The full license can be found in the LICENSE.txt file.
 */

#include <string.h>
#include <stdio.h>

#include <cglm/vec3.h>
// #include <cglm/vec4.h>

#include "div.h"
#include "gui.h"

static void div_recalculate_area(struct widget *wgt)
{
    (void)wgt;
}

static void div_redraw_widget(struct widget *wgt, struct window *win)
{
    struct div *impl = WIDGET_GET_IMPL(struct div, wgt);

    /* TODO: Update children. */
    /* TODO: Add styling. */

    renderer_draw_quad(&win->rend,
                       (vec3){wgt->av_x, wgt->av_y, 0.0f},
                       (vec3){wgt->av_width, wgt->av_height, 1.0f},
                       impl->background_color);

    printf(INFO "div_redraw_widget()\n");
}

void div_create(struct div *div)
{
    struct widget *base = WIDGET_GET_BASE(div);

    memset(div, 0, sizeof(*div));
    base->recalculate_area = div_recalculate_area;
    base->redraw_widget = div_redraw_widget;
}
