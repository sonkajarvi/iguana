/**
 * Copyright (c) 2026, sonkajarvi
 *
 * Licensed under the BSD 2-Clause License.
 * The full license can be found in the LICENSE.txt file.
 */

#include <X11/X.h>
#include <stdio.h>
#include <string.h>

#include <gui/window.h>

#include "../internal.h"

static Atom __X11_wm_delete_window;

int X11_window_create(struct window *wnd, int width, int height, const char *title)
{
    XVisualInfo *vi;
    XSetWindowAttributes swa;

    wnd->display = XOpenDisplay(NULL);
    if (!wnd->display) {
        printf(INFO "window: failed to open display\n");
        return -1;
    }

    vi = GLX_get_visual(wnd);
    if (!vi) {
        printf(ERROR "window: failed to get VisualInfo from FB config\n");
        XCloseDisplay(wnd->display);
    }

    wnd->root = DefaultRootWindow(wnd->display);

    memset(&swa, 0, sizeof(swa));
    swa.colormap = XCreateColormap(wnd->display, wnd->root, vi->visual, AllocNone);
    swa.background_pixmap = None;
    swa.border_pixel = 0;
    swa.event_mask = StructureNotifyMask;

    wnd->window = XCreateWindow(
        wnd->display,
        wnd->root,
        0,
        0,
        width,
        height,
        0,
        vi->depth,
        InputOutput,
        vi->visual,
        CWBorderPixel | CWColormap | CWEventMask,
        &swa
    );

    XFree(vi);

    if (GLX_create_context(wnd) != 0) {
        printf(ERROR "GLX: failed to create context\n");
        XDestroyWindow(wnd->display, wnd->window);
        XCloseDisplay(wnd->display);
        return -1;
    }

    __X11_wm_delete_window = XInternAtom(wnd->display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(wnd->display, wnd->window, &__X11_wm_delete_window, 1);

    XStoreName(wnd->display, wnd->window, title);

    printf(INFO "created new window: width %d, height %d\n", width, height);
    return 0;
}

void X11_window_destroy(struct window *wnd)
{
    GLX_destroy_context(wnd);
    GL_close_handle();

    XDestroyWindow(wnd->display, wnd->window);
    XCloseDisplay(wnd->display);
}

void X11_window_show(struct window *wnd)
{
    XMapWindow(wnd->display, wnd->window);

    wnd->should_close = false;
    wnd->should_repaint = true;
}

void X11_poll_events(struct window *wnd)
{
    XEvent event;

    while (QLength(wnd->display)) {
        XNextEvent(wnd->display, &event);

        switch (event.type) {
        case ClientMessage:
            if ((Atom)event.xclient.data.l[0] == __X11_wm_delete_window)
                wnd->should_close = true;
            break;

        case ConfigureNotify:
            wnd->should_repaint = true;
            break;

        default:
            break;
        }
    }
}
