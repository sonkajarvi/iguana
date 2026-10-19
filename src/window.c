/**
 * Copyright (c) 2025-2026, sonkajarvi
 *
 * Licensed under the BSD 2-Clause License.
 * The full license can be found in the LICENSE.txt file.
 */

#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iguana/event.h>
#include <iguana/rect.h>
#include <iguana/platform_window.h>
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
        if (igu_rect_point_in_rect(rect,
                                   window->pwindow->mouse_x,
                                   window->pwindow->mouse_y))
            widget = window->hovered;
    }

    return test_hit_vertical(widget,
                             window->pwindow->mouse_x,
                             window->pwindow->mouse_y);
}

static void igu_window_compute_hovered_widget(struct igu_window *window)
{
    struct igu_widget *hit;

    if (window->pwindow->mouse_x_delta == 0 &&
        window->pwindow->mouse_y_delta == 0)
        return;

    hit = igu_window_hit_test(window);
    if (hit != window->hovered) {
        igu_widget_invoke_event(hit, on_mouse_enter);
        igu_widget_invoke_event(window->hovered, on_mouse_exit);
    }
    window->hovered = hit;
}

static void igu_window_handle_events(struct igu_window *window)
{
    struct igu_event event;
    int ret;

    ret = igu_event_queue_pop(&window->pwindow->event_queue, &event);
    if (ret == -ENOENT)
        return;
    if (ret < 0) {
        printf(ERROR "WINDOW: failed to pop event from the queue\n");
        return;
    }

    switch (event.type) {
    case IGU_EVENT_TYPE_MOUSE:
        break;

    case IGU_EVENT_TYPE_MOUSE_MOVE:
        window->pwindow->mouse_x_prev = window->pwindow->mouse_x;
        window->pwindow->mouse_y_prev = window->pwindow->mouse_y;

        window->pwindow->mouse_x = event.mouse_move.x;
        window->pwindow->mouse_y = event.mouse_move.y;

        window->pwindow->mouse_x_delta = window->pwindow->mouse_x -
                                         window->pwindow->mouse_x_prev;
        window->pwindow->mouse_y_delta = window->pwindow->mouse_y -
                                         window->pwindow->mouse_y_prev;
        break;

    case IGU_EVENT_TYPE_MOUSE_SCROLL:
        break;

    case IGU_EVENT_TYPE_WINDOW_MOVE:
        window->pwindow->x = event.window_move.x;
        window->pwindow->y = event.window_move.y;
        break;

    case IGU_EVENT_TYPE_WINDOW_RESIZE:
        window->pwindow->width = event.window_resize.width;
        window->pwindow->height = event.window_resize.height;

        window->should_redraw = true;
        break;

    default:
        break;
    }
}

int igu_window_exec(struct igu_window *window)
{
    igu_platform_window_show(window->pwindow);
    window->should_close = false;
    window->should_redraw = true;

    if (window->widget)
        window->widget->window = window;

    while (!window->should_close) {
        igu_platform_window_poll_events(window->pwindow);

        if (window->should_redraw) {
            renderer_new_frame(window->renderer, window);

            if (window->widget) {
                IGU_WIDTH(window->widget->rect) = window->pwindow->width;
                IGU_HEIGHT(window->widget->rect) = window->pwindow->height;

                igu_widget_relayout(window->widget);
                igu_widget_redraw(window->widget);
            }

            renderer_flush_vertices(window->renderer);
            window->should_redraw = false;

            igu_platform_window_swap_buffers(window->pwindow);
        }

        igu_window_compute_hovered_widget(window);

        igu_window_handle_events(window);
    }

    igu_platform_window_destroy(window->pwindow);

    return 0;
}

struct igu_window *igu_window_create(int width, int height)
{
    struct igu_window *window;

    window = malloc(sizeof(*window));
    if (!window)
        return NULL;

    memset(window, 0, sizeof(*window));
    window->should_close = true;
    printf(INFO "WINDOW: allocated memory for window structure\n");

    window->pwindow = igu_platform_window_create(width, height);
    if (!window->pwindow)
        goto err_free;

    window->renderer = gl_renderer_create();
    if (!window->renderer)
        goto err_destroy;

    window->pwindow->cursor = IGU_CURSOR_ARROW;
    window->should_close = true;
    window->should_redraw = false;

    return window;

err_destroy:
    igu_platform_window_destroy(window->pwindow);
err_free:
    free(window);
    return NULL;
}
