/**
 * Copyright (c) 2025, sonkajarvi
 *
 * Licensed under the BSD 2-Clause License.
 * The full license can be found in the LICENSE.txt file.
 */

#include <stdio.h>

#include <cglm/vec3.h>
#include <cglm/vec4.h>

#include "gui.h"

void widget_update(struct widget *wgt, struct window *win)
{
    /* TODO: Update children. */
    /* TODO: Add styling. */

    renderer_draw_quad(&win->rend,
                       (vec3){wgt->x, wgt->y, 0.0f},
                       (vec3){wgt->width, wgt->height, 1.0f},
                       (vec4){1.0f, 1.0f, 1.0f, 1.0f});

    printf(INFO "Updated widget\n");
}
