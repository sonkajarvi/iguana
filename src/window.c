/**
 * Copyright (c) 2025-2026, sonkajarvi
 *
 * Licensed under the BSD 2-Clause License.
 * The full license can be found in the LICENSE.txt file.
 */

#include <gui/window.h>

#include "renderer.h"

int window_exec(struct window *window)
{
    XWindowAttributes wa;

    x11_window_show(window);

    if (window->widget)
        window->widget->window = window;

    while (!window->should_close) {
        x11_window_poll_events(window);

        XGetWindowAttributes(window->display, window->window, &wa);

        if (window->should_redraw) {
            renderer_new_frame(window->renderer, window);

            if (window->widget) {
                WIDTH(window->widget->rect) = wa.width;
                HEIGHT(window->widget->rect) = wa.height;

                WIDGET_RELAYOUT(window->widget);
                WIDGET_REPAINT(window->widget);
            }

            renderer_flush_vertices(window->renderer);
            // window->should_redraw = false;

            x11_window_swap_buffers(window);
        }
    }

    x11_window_destroy(window);

    return 0;
}
