/**
 * Copyright (c) 2025-2026, sonkajarvi
 *
 * Licensed under the BSD 2-Clause License.
 * The full license can be found in the LICENSE.txt file.
 */

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <X11/Xlib.h>
#include <X11/Xcursor/Xcursor.h>
#include <X11/extensions/Xfixes.h>

#include <iguana/event.h>
#include <iguana/window.h>

static Atom wm_delete_window;

/* time is in nanoseconds */
static uint64_t get_time(void)
{
    struct timespec ts;

    clock_gettime(CLOCK_REALTIME, &ts);
    return (uint64_t)ts.tv_sec * 1000000000 + (uint64_t)ts.tv_nsec;
}

/* time is in seconds */
double x11_window_get_time(struct igu_window *window)
{
    if (window)
        return (double)(get_time() - window->time_offset) / 1000000000.0;
    else
        return 0.0;
}

static int __x11_window_create(struct igu_window *window, int width, int height)
{
    XVisualInfo *vi;
    XSetWindowAttributes swa;

    if (!window)
        return -1;

    window->display = XOpenDisplay(NULL);
    if (!window->display) {
        printf(INFO "X11 WINDOW: failed to open display\n");
        return -1;
    }

    vi = glx_get_visual(window);
    if (!vi) {
        printf(ERROR "X11 WINDOW: failed to get VisualInfo from FB config\n");
        XCloseDisplay(window->display);
    }

    window->root = DefaultRootWindow(window->display);

    memset(&swa, 0, sizeof(swa));
    swa.colormap = XCreateColormap(window->display,
                                   window->root, vi->visual, AllocNone);
    swa.background_pixmap = None;
    swa.border_pixel = 0;
    swa.event_mask = StructureNotifyMask | PointerMotionMask |
                     ButtonPressMask | ButtonReleaseMask;

    window->window = XCreateWindow(
        window->display,
        window->root,
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

    if (glx_create_context(window) != 0) {
        XDestroyWindow(window->display, window->window);
        XCloseDisplay(window->display);
        return -1;
    }

    wm_delete_window = XInternAtom(window->display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(window->display, window->window, &wm_delete_window, 1);

    window->time_offset = get_time();
    return 0;
}

int x11_window_create(struct igu_window *window, int width, int height)
{
    if (!window)
        return -1;

    if (gl_open_handle() != 0)
        return -1;

    if (__x11_window_create(window, width, height) != 0)
        return -1;

    if (gl_load_functions() != 0)
        return -1;

    igu_event_queue_push_window_resize_event(&window->event_queue,
                                             width, height);

    printf(INFO "X11 WINDOW: created new window: width %d, height %d\n", width,
           height);

    return 0;
}

void x11_window_destroy(struct igu_window *window)
{
    if (!window)
        return;

    glx_destroy_context(window);

    XDestroyWindow(window->display, window->window);
    XCloseDisplay(window->display);

    gl_close_handle();

    igu_event_queue_free(&window->event_queue);

    printf(INFO "X11 WINDOW: window destroyed\n");
}

void x11_window_show(struct igu_window *window)
{
    if (!window)
        return;

    XMapWindow(window->display, window->window);
    window->should_close = false;
    window->should_redraw = true;
}

static void x11_window_handle_mouse_events(struct igu_window *window,
                                           XEvent *event)
{
    int button = event->xbutton.button;

    /*
     * Handle mouse wheel scroll events here separately.
     */
    if (button == Button4 || button == Button5) {
        igu_event_queue_push_mouse_scroll_event(&window->event_queue,
                                                (button == Button4) * 2 - 1);
        return;
    }

    /*
     * TODO: Handle mouse wheel tilt events.
     */
    if (button == 6 || button == 7)
        return;

    switch (button) {
    case Button1: button = IGU_MOUSE_LEFT; break;
    case Button2: button = IGU_MOUSE_MIDDLE; break;
    case Button3: button = IGU_MOUSE_RIGHT; break;
    /*
     * The extra mouse buttons aren't named in "X.h" like the rest are,
     * but they are mapped to 8, 9 and so on...
     */
    case 8:       button = IGU_MOUSE_X1; break;
    case 9:       button = IGU_MOUSE_X2; break;
    case 10:      button = IGU_MOUSE_X3; break;
    default:      return;
    }

    igu_event_queue_push_mouse_event(&window->event_queue, button,
                                     event->type == ButtonPress);
}

static void x11_window_handle_mouse_move_events(struct igu_window *window,
                                                XEvent *event)
{
    igu_event_queue_push_mouse_move_event(&window->event_queue,
                                          event->xmotion.x,
                                          event->xmotion.y);
}

static void x11_window_handle_window_events(struct igu_window *window,
                                            XEvent *event)
{
    igu_event_queue_push_window_move_event(&window->event_queue,
                                           event->xconfigure.x,
                                           event->xconfigure.y);

    igu_event_queue_push_window_resize_event(&window->event_queue,
                                             event->xconfigure.width,
                                             event->xconfigure.height);
}

static void __x11_window_poll_events(struct igu_window *window)
{
    XEvent event;

    if (!window)
        return;

    if (!XQLength(window->display))
        XFlush(window->display);

    while (XQLength(window->display)) {
        XNextEvent(window->display, &event);

        switch (event.type) {
        case ClientMessage:
            if ((Atom)event.xclient.data.l[0] == wm_delete_window) {
                window->should_close = true;
                return;
            }
            break;

        case ButtonPress:
        case ButtonRelease:
            x11_window_handle_mouse_events(window, &event);
            break;

        case MotionNotify:
            x11_window_handle_mouse_move_events(window, &event);
            break;

        case ConfigureNotify:
            x11_window_handle_window_events(window, &event);
            break;
        }
    }
}

void x11_window_poll_events(struct igu_window *window)
{
    static double last = 0.0;

    if (!window)
        return;

    double now = x11_window_get_time(window);
    window->delta_time = (float)(now - last);
    last = now;

    __x11_window_poll_events(window);
}

void x11_window_swap_buffers(struct igu_window *window)
{
    if (window)
        glXSwapBuffers(window->display, window->window);
}

static const char *x_cursor_files[] = {
    [IGU_CURSOR_NONE]          = NULL,
    [IGU_CURSOR_ARROW]         = "arrow",

    [IGU_CURSOR_CONTEXT_MENU]  = "context-menu",
    [IGU_CURSOR_HELP]          = "question_arrow",
    [IGU_CURSOR_POINTER]       = "hand2",
    [IGU_CURSOR_PROGRESS]      = "left_ptr_watch",
    [IGU_CURSOR_WAIT]          = "watch",

    [IGU_CURSOR_CELL]          = "cell",
    [IGU_CURSOR_CROSSHAIR]     = "crosshair",
    [IGU_CURSOR_TEXT]          = "xterm",
    [IGU_CURSOR_VERTICAL_TEXT] = "vertical-text",

    [IGU_CURSOR_ALIAS]         = "dnd-link",
    [IGU_CURSOR_COPY]          = "copy",
    [IGU_CURSOR_MOVE]          = "all-scroll",
    [IGU_CURSOR_NO_DROP]       = "no-drop",
    [IGU_CURSOR_NOT_ALLOWED]   = "not-allowed",
    [IGU_CURSOR_GRAB]          = "grab",
    [IGU_CURSOR_GRABBING]      = "grabbing",

    [IGU_CURSOR_ALL_SCROLL]    = "all-scroll",
    [IGU_CURSOR_COLUMN_RESIZE] = "sb_h_double_arrow",
    [IGU_CURSOR_ROW_RESIZE]    = "sb_v_double_arrow",
    [IGU_CURSOR_N_RESIZE]      = "top_side",
    [IGU_CURSOR_E_RESIZE]      = "right_side",
    [IGU_CURSOR_S_RESIZE]      = "bottom_side",
    [IGU_CURSOR_W_RESIZE]      = "left_side",
    [IGU_CURSOR_NE_RESIZE]     = "top_right_corner",
    [IGU_CURSOR_NW_RESIZE]     = "top_left_corner",
    [IGU_CURSOR_SE_RESIZE]     = "bottom_right_corner",
    [IGU_CURSOR_SW_RESIZE]     = "bottom_left_corner",
    [IGU_CURSOR_EW_RESIZE]     = "sb_h_double_arrow",
    [IGU_CURSOR_NS_RESIZE]     = "sb_v_double_arrow",
    [IGU_CURSOR_NESW_RESIZE]   = "bottom_left_corner",
    [IGU_CURSOR_NWSE_RESIZE]   = "bottom_right_corner",

    [IGU_CURSOR_ZOOM_IN]       = "zoom-in",
    [IGU_CURSOR_ZOOM_OUT]      = "zoom-out",
};

int x11_window_set_cursor(struct igu_window *window, unsigned int cursor)
{
    Cursor x_cursor;

    if (!window || cursor > IGU_CURSOR_LAST)
        return -EINVAL;

    if (cursor == IGU_CURSOR_NONE) {
        XFixesHideCursor(window->display, window->window);
        goto out;
    }

    x_cursor = XcursorLibraryLoadCursor(window->display,
                                        x_cursor_files[cursor]);
    XDefineCursor(window->display, window->window, x_cursor);

    if (window->cursor == IGU_CURSOR_NONE)
        XFixesShowCursor(window->display, window->window);

out:
    window->cursor = cursor;

    return 0;
}
