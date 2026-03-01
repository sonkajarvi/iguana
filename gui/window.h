/**
 * Copyright (c) 2026, sonkajarvi
 *
 * Licensed under the BSD 2-Clause License.
 * The full license can be found in the LICENSE.txt file.
 */

#ifndef __WINDOW_H
#define __WINDOW_H

#include <stdbool.h>
#include <stdint.h>

#include <X11/X.h>

#include <cglm/types.h>
#include <glad/gl.h>
#include <glad/glx.h>

#include <gui/widget.h>

#define INFO    "[info] "
#define ERROR   "[error] "

#define STYLE_FILL 0
#define STYLE_STROKE 1
#define STYLE_TEXTURED_FILL 2

struct rect {
    vec2 position;
    vec2 size;
    vec4 radius;
};

struct style {
    int style;
    uint32_t color;

    /* stroke */
    float st_width;

    /* textured fill */
    struct texture *tx_texture;
    vec4 tx_coords;
};

struct vertex {
    vec2 position;
    vec4 color;
    vec2 center;
    vec2 size;
    vec4 radius;
    vec2 texture_coords;
    float texture_index;
    float stroke_width;
};

struct image {
    uint8_t *data;
    int width;
    int height;
};

struct texture {
    int width;
    int height;
    int index;

    /* OpenGL */
    GLuint id;
};

struct painter {
    struct vertex *v_buf;       /* also used as a buffer for shader logs */
    struct vertex *v_ptr;

    struct texture *tex_array[4];
    int tex_count;
    struct texture white_texture;

    /* opengl */
    GLuint program;
    GLuint vao;
    GLuint vbo;
};

struct window {
    struct widget *widget;
    struct painter painter;
    bool should_close;
    bool should_repaint;

    /* styling */
    uint32_t background_color;

    /* x11 */
    Display *display;
    Window root;
    Window window;
    uint64_t time_offset;

    /* glx */
    GLXContext context;
    GLXFBConfig fbconfig;
};

struct window *window_new(int width, int height, const char *title);
int window_exec(struct window *wnd);

#endif /* !__WINDOW_H */
