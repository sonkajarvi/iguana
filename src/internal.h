/**
 * Copyright (c) 2026, sonkajarvi
 *
 * Licensed under the BSD 2-Clause License.
 * The full license can be found in the LICENSE.txt file.
 */

#ifndef __INTERNAL_H
#define __INTERNAL_H

#include <gui/window.h>

/* x11 window */

int X11_window_create(struct window *wnd, int width, int height, const char *title);
void X11_window_destroy(struct window *wnd);
void X11_window_show(struct window *wnd);
void X11_poll_events(struct window *wnd);

/* gl context */

int GL_open_handle(void);
void GL_close_handle(void);
GLADapiproc GL_get_proc_address(const char *name);
int GL_load_functions(struct window *wnd);

/* glx context */

int GLX_load_functions(struct window *wnd, int screen);
int GLX_choose_fbconfig(struct window *wnd, int screen, GLXFBConfig *fbc);
int GLX_create_context(struct window *wnd);
void GLX_destroy_context(struct window *wnd);
XVisualInfo *GLX_get_visual(struct window *wnd);

/* gl painter */

int GL_painter_create(struct painter *paint);
void GL_painter_destroy(struct painter *paint);
void GL_painter_begin_frame(struct painter *paint, struct window *wnd);
void GL_painter_end_frame(struct painter *paint);
void GL_painter_draw_rect(struct painter *paint, struct rect *rect, struct style *style);

#endif /* __INTERNAL_H */
