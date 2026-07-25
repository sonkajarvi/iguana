/**
 * Copyright (c) 2025-2026, sonkajarvi
 *
 * Licensed under the BSD 2-Clause License.
 * The full license can be found in the LICENSE.txt file.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <iguana/rect.h>
#include <iguana/widget.h>
#include <iguana/window.h>

#include "gl/gl_renderer.h"
#include "renderer.h"

static struct igu_widget *test_hit_horizontal(struct igu_widget *widget,
                                              float x, float y)
{
    igu_rect_t rect;

loop:
    if (!widget)
        return NULL;

    igu_widget_rect_with_margin(widget, rect);
    if (igu_rect_point_in_rect(rect, x, y))
        return widget;

    widget = widget->next;
    goto loop;
}

static struct igu_widget *test_hit_vertical(struct igu_widget *widget,
                                            float x, float y)
{
    struct igu_widget *prev = NULL;

loop:
    if (!widget)
        return prev;

    widget = test_hit_horizontal(widget, x, y);
    if (!widget)
        return prev;

    prev = widget;
    widget = widget->children;
    goto loop;
}

static struct igu_widget *igu_window_hit_test(struct igu_window *window)
{
    struct igu_widget *widget = window->widget;
    igu_rect_t rect;

    if (window->hovered) {
        igu_widget_rect_with_margin(window->hovered, rect);
        if (igu_rect_point_in_rect(rect, window->mouse_x, window->mouse_y))
            widget = window->hovered;
    }

    return test_hit_vertical(widget, window->mouse_x, window->mouse_y);
}

int igu_window_exec(struct igu_window *window)
{
    x11_window_show(window);

    if (window->widget)
        window->widget->window = window;

    while (!window->should_close) {
        x11_window_poll_events(window);

        if (window->should_redraw) {
            renderer_new_frame(window->renderer, window);

            if (window->widget) {
                IGU_WIDTH(window->widget->rect) = window->width;
                IGU_HEIGHT(window->widget->rect) = window->height;

                igu_widget_relayout(window->widget);
                igu_widget_redraw(window->widget);
            }

            renderer_flush_vertices(window->renderer);
            // window->should_redraw = false;

            x11_window_swap_buffers(window);
        }

        if (window->mouse_x_delta || window->mouse_y_delta)
            window->hovered = igu_window_hit_test(window);
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