/**
 * Copyright (c) 2026, sonkajarvi
 *
 * Licensed under the BSD 2-Clause License.
 * The full license can be found in the LICENSE.txt file.
 */

#ifndef IGUANA_PLATFORM_WINDOW_H
#define IGUANA_PLATFORM_WINDOW_H

#include <stdint.h>

#include <X11/X.h>

#include <glad/glx.h>

#include <iguana/event.h>

/**
 * struct igu_platform_window - the base window structure
 *
 * TODO: Separate the X11 and GLX stuff from the generic stuff.
 */
struct igu_platform_window {
    struct igu_event_queue event_queue;

    int x, y, width, height;
    float delta_time;
    int mouse_x, mouse_x_prev, mouse_x_delta;
    int mouse_y, mouse_y_prev, mouse_y_delta;

    unsigned int cursor;

    struct {
        Display *display;
        Window root;
        Window window;
        uint64_t time_offset;
    } x11;
    struct {
        GLXContext context;
        GLXFBConfig fbconfig;
    } glx;
};

struct igu_platform_window *igu_platform_window_create(int height, int width);
void igu_platform_window_destroy(struct igu_platform_window *pwindow);

void igu_platform_window_poll_events(struct igu_platform_window *pwindow);
void igu_platform_window_swap_buffers(struct igu_platform_window *pwindow);

double igu_platform_window_get_time(struct igu_platform_window *pwindow);

void igu_platform_window_show(struct igu_platform_window *pwindow);

/**
 * igu_platform_set_cursor - modify the state of the window cursor
 * @pwindow:        The window the cursor belongs to
 * @cursor:         New cursor state
 *
 * Sets the cursor to the given shape, unless IGU_CURSOR_NONE is passed, in
 * which case the cursor is hidden. Passing any of the other options makes
 * the cursor visible again.
 *
 * Returns 0 if successful, a negative error code otherwise.
 */
int igu_platform_window_set_cursor(struct igu_platform_window *pwindow,
                                   unsigned int cursor);

/*
 * GLX context
 */
XVisualInfo *glx_get_visual(struct igu_platform_window *pwindow);
int glx_create_context(struct igu_platform_window *pwindow);
void glx_destroy_context(struct igu_platform_window *pwindow);

/*
 * GL context
 */
GLADapiproc gl_get_proc_address(const char *name);
int gl_load_functions(void);
int gl_open_handle(void);
void gl_close_handle(void);

#endif /* !IGUANA_PLATFORM_WINDOW_H */
