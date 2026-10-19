/**
 * Copyright (c) 2025-2026, sonkajarvi
 *
 * Licensed under the BSD 2-Clause License.
 * The full license can be found in the LICENSE.txt file.
 */

#ifndef _IGUANA_WINDOW_H
#define _IGUANA_WINDOW_H

#include <stdbool.h>

#include <iguana/event.h>
#include <iguana/widget.h>

#define INFO    "[INFO] "
#define ERROR   "[ERROR] "

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

char *igu_read_file(const char *path);

enum {
    IGU_CURSOR_NONE = 0,
    IGU_CURSOR_ARROW,

    IGU_CURSOR_CONTEXT_MENU,
    IGU_CURSOR_HELP,
    IGU_CURSOR_POINTER,
    IGU_CURSOR_PROGRESS,
    IGU_CURSOR_WAIT,

    IGU_CURSOR_CELL,
    IGU_CURSOR_CROSSHAIR,
    IGU_CURSOR_TEXT,
    IGU_CURSOR_VERTICAL_TEXT,

    IGU_CURSOR_ALIAS,
    IGU_CURSOR_COPY,
    IGU_CURSOR_MOVE,
    IGU_CURSOR_NO_DROP,
    IGU_CURSOR_NOT_ALLOWED,
    IGU_CURSOR_GRAB,
    IGU_CURSOR_GRABBING,

    IGU_CURSOR_ALL_SCROLL,
    IGU_CURSOR_COLUMN_RESIZE,
    IGU_CURSOR_ROW_RESIZE,
    IGU_CURSOR_N_RESIZE,
    IGU_CURSOR_E_RESIZE,
    IGU_CURSOR_S_RESIZE,
    IGU_CURSOR_W_RESIZE,
    IGU_CURSOR_NE_RESIZE,
    IGU_CURSOR_NW_RESIZE,
    IGU_CURSOR_SE_RESIZE,
    IGU_CURSOR_SW_RESIZE,
    IGU_CURSOR_EW_RESIZE,
    IGU_CURSOR_NS_RESIZE,
    IGU_CURSOR_NESW_RESIZE,
    IGU_CURSOR_NWSE_RESIZE,

    IGU_CURSOR_ZOOM_IN,
    IGU_CURSOR_ZOOM_OUT,

    IGU_CURSOR_LAST = IGU_CURSOR_ZOOM_OUT,
};

/**
 * struct igu_window - The window structure
 */
struct igu_window {
    bool should_close;
    bool should_redraw;

    struct igu_platform_window *pwindow;
    struct renderer *renderer;

    struct igu_widget *widget;
    struct igu_widget *hovered;
};

int igu_window_exec(struct igu_window *window);
struct igu_window *igu_window_create(int width, int height);

#endif /* !_IGUANA_WINDOW_H */
