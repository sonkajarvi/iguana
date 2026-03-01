/**
 * Copyright (c) 2025, sonkajarvi
 *
 * Licensed under the BSD 2-Clause License.
 * The full license can be found in the LICENSE.txt file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <gui/window.h>
#include <gui/color.h>

#include "internal.h"

/* time is in nanoseconds */
static uint64_t __get_time(void)
{
    struct timespec ts;

    clock_gettime(CLOCK_REALTIME, &ts);
    return (uint64_t)ts.tv_sec * 1000000000 + (uint64_t)ts.tv_nsec;
}

struct window *window_new(int width, int height, const char *title)
{
    struct window *wnd;

    wnd = malloc(sizeof(*wnd));
    if (!wnd)
        return NULL;

    memset(wnd, 0, sizeof(*wnd));

    if (GL_open_handle() != 0) {
        free(wnd);
        return NULL;
    }
    if (X11_window_create(wnd, width, height, title) != 0) {
        free(wnd);
        return NULL;
    }

    if (GL_load_functions(wnd) != 0) {
        X11_window_destroy(wnd);
        free(wnd);
        return NULL;
    }
    if (GL_painter_create(&wnd->painter) != 0) {
        X11_window_destroy(wnd);
        free(wnd);
        return NULL;
    }

    wnd->should_close = true;
    wnd->should_repaint = false;
    wnd->background_color = COLOR_WHITE;
    wnd->time_offset = __get_time();

    printf(INFO "created new window: width %d, height %d\n", width, height);
    return wnd;
}

int window_exec(struct window *wnd)
{
    XWindowAttributes wa;

    X11_window_show(wnd);

    if (wnd->widget)
        wnd->widget->window = wnd;

    while (!wnd->should_close) {
        X11_poll_events(wnd);

        XGetWindowAttributes(wnd->display, wnd->window, &wa);
        printf(INFO "%d %d\n", wa.width, wa.height);

        if (wnd->should_repaint) {
            GL_painter_begin_frame(&wnd->painter, wnd);

            if (wnd->widget) {
                wnd->widget->width = wa.width;
                wnd->widget->height = wa.height;
                WIDGET_RELAYOUT(wnd->widget);

                WIDGET_REPAINT(wnd->widget);
            }

            GL_painter_end_frame(&wnd->painter);
            wnd->should_repaint = false;

            glXSwapBuffers(wnd->display, wnd->window);
        }
    }

    X11_window_destroy(wnd);
    return 0;
}
