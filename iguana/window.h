/**
 * Copyright (c) 2025-2026, sonkajarvi
 *
 * Licensed under the BSD 2-Clause License.
 * The full license can be found in the LICENSE.txt file.
 */

#ifndef _IGUANA_WINDOW_H
#define _IGUANA_WINDOW_H

#include <stdbool.h>
#include <stdint.h>

#include <X11/X.h>

#include <glad/glx.h>

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
 *
 * TODO: Separate the X11 and GLX stuff from the generic stuff.
 */
struct igu_window {
    struct igu_widget *widget;

    struct renderer *renderer;
    struct igu_event_queue event_queue;
    bool should_close;
    bool should_redraw;
    float delta_time;
    int fps;

    int x, y;
    int width, height;
    int mouse_x, mouse_x_delta, mouse_x_prev;
    int mouse_y, mouse_y_delta, mouse_y_prev;
    struct igu_widget *hovered;

    /* X11 */
    Display *display;
    Window root;
    Window window;
    uint64_t time_offset;

    /* GLX */
    GLXContext context;
    GLXFBConfig fbconfig;
};

int igu_window_exec(struct igu_window *window);
struct igu_window *igu_window_create(int width, int height);

int x11_window_create(struct igu_window *window, int width, int height);
void x11_window_destroy(struct igu_window *window);
void x11_window_show(struct igu_window *window);
void x11_window_swap_buffers(struct igu_window *window);
void x11_window_poll_events(struct igu_window *window);

double x11_window_get_time(struct igu_window *window);

/*
 * GLX context
 */
XVisualInfo *glx_get_visual(struct igu_window *window);
int glx_create_context(struct igu_window *window);
void glx_destroy_context(struct igu_window *window);

/*
 * GL context
 */
GLADapiproc gl_get_proc_address(const char *name);
int gl_load_functions(void);
int gl_open_handle(void);
void gl_close_handle(void);

#endif /* !_IGUANA_WINDOW_H */
