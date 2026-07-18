/**
 * Copyright (c) 2025-2026, sonkajarvi
 *
 * Licensed under the BSD 2-Clause License.
 * The full license can be found in the LICENSE.txt file.
 */

#include <stdlib.h>

#include <iguana/window.h>

#include "gl/gl_renderer.h"
#include "renderer.h"

int igu_window_exec(struct igu_window *window)
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
                IGU_WIDTH(window->widget->rect) = wa.width;
                IGU_HEIGHT(window->widget->rect) = wa.height;

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

struct igu_window *igu_window_create(int width, int height)
{
    struct igu_window *window;

    window = malloc(sizeof(*window));
    if (!window)
        return NULL;

    if (x11_window_create(window, width, height) != 0)
        goto err_free;

    window->renderer = gl_renderer_create();
    if (!window->renderer)
        goto err_destroy;

    return window;

err_destroy:
    x11_window_destroy(window);
err_free:
    free(window);
    return NULL;
}