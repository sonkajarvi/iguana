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

#include <iguana/widget.h>

#define INFO    "[INFO] "
#define ERROR   "[ERROR] "

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

char *igu_read_file(const char *path);

/**
 * struct igu_window - The window structure
 *
 * TODO: Separate the X11 and GLX stuff from the generic stuff.
 */
struct igu_window {
    struct igu_widget *widget;

    struct renderer *renderer;
    bool should_close;
    bool should_redraw;
    float delta_time;
    int fps;

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
