/**
 * Copyright (c) 2026, sonkajarvi
 *
 * Licensed under the BSD 2-Clause License.
 * The full license can be found in the LICENSE.txt file.
 */

#include <X11/Xlib.h>
#include <stdio.h>
#include <string.h>

#include <cglm/vec2.h>
#include <cglm/vec4.h>

#include <gui/color.h>
#include <gui/window.h>

#include "internal.h"
#include "utils.h"

#define min(a, b)                \
    ({                           \
        __typeof__(a) __a = (a); \
        __typeof__(a) __b = (b); \
        __a < __b ? __a : __b;   \
    })

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

/* length of the vertex buffer in bytes */
#define VERTEX_BUFFER_LENGTH 4096

static int __GL_texture_create(struct texture *tex, struct image *img)
{
    glGenTextures(1, &tex->id);
    glBindTexture(GL_TEXTURE_2D, tex->id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
        img->width, img->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->data);
    glGenerateMipmap(GL_TEXTURE_2D);

    tex->width = img->width;
    tex->height = img->height;
    tex->index = -1;

    printf(INFO "GL: created texture, id %u\n", tex->id);

    return 0;
}

static void __GL_texture_destroy(struct texture *tex)
{
    glDeleteTextures(1, &tex->id);
}

static GLuint __GL_create_shader(struct painter *paint, const char *v_path, const char *f_path)
{
    int ret;
    char *v_source, *f_source;
    GLuint program, v_shader, f_shader;

    v_source = f_source = NULL;
    program = v_shader = f_shader = 0;

    /* Vertex shader */

    v_source = read_file(v_path);
    if (!v_source)
        goto out_cleanup;

    v_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(v_shader, 1, (void *)&v_source, NULL);
    glCompileShader(v_shader);

    glGetShaderiv(v_shader, GL_COMPILE_STATUS, &ret);
    if (!ret) {
        glGetShaderInfoLog(v_shader, VERTEX_BUFFER_LENGTH, NULL, (void *)paint->v_buf);
        printf(ERROR "GL: failed to compile vertex shader:\n%s", (char *)paint->v_buf);
        goto out_cleanup;
    }

    printf(INFO "GL: compiled vertex shader: path %s\n", v_path);

    /* Fragment shader */

    f_source = read_file(f_path);
    if (!f_source)
        goto out_cleanup;

    f_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(f_shader, 1, (void *)&f_source, NULL);
    glCompileShader(f_shader);

    glGetShaderiv(f_shader, GL_COMPILE_STATUS, &ret);
    if (!ret) {
        glGetShaderInfoLog(f_shader, VERTEX_BUFFER_LENGTH, NULL, (void *)paint->v_buf);
        printf(ERROR "GL: failed to compile fragment shader:\n%s", (char *)paint->v_buf);
        goto out_cleanup;
    }

    printf(INFO "GL: compiled fragment shader: path %s\n", f_path);

    /* Shader program */

    program = glCreateProgram();
    glAttachShader(program, v_shader);
    glAttachShader(program, f_shader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &ret);
    if (!ret) {
        glGetProgramInfoLog(program, VERTEX_BUFFER_LENGTH, NULL, (void *)paint->v_buf);
        printf(ERROR "GL: failed to create shader program:\n%s", (char *)paint->v_buf);
        goto out_cleanup;
    }

    printf(INFO "GL: created shader program: id %u\n", program);
    return program;

out_cleanup:
    free(v_source);
    glDeleteShader(v_shader);

    free(f_source);
    glDeleteShader(f_shader);

    glDeleteProgram(program);

    return 0;
}

static void __GL_painter_use_texture(struct painter *paint, struct texture *tex)
{
    /* TODO: this should not fail silently */
    if (paint->tex_count == ARRAY_SIZE(paint->tex_array))
        return;

    if (tex->index != -1)
        return;

    paint->tex_array[paint->tex_count] = tex;
    tex->index = paint->tex_count;
    paint->tex_count++;
}

void GL_painter_begin_frame(struct painter *paint, struct window *wnd)
{
    float color[3];
    XWindowAttributes wa;

    XGetWindowAttributes(wnd->display, wnd->window, &wa);
    glViewport(0, 0, wa.width, wa.height);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    color_to_3f(wnd->background_color, color);
    glClearColor(color[0], color[1], color[2], 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(paint->program);

    GLfloat size[2];
    size[0] = wa.width;
    size[1] = wa.height;
    glUniform2fv(glGetUniformLocation(paint->program, "u_WindowSize"), 1, size);

    paint->v_ptr = paint->v_buf;

    paint->tex_count = 0;
    __GL_painter_use_texture(paint, &paint->white_texture);
}

void GL_painter_end_frame(struct painter *paint)
{
    static const GLint array[] = {0, 1, 2, 3};
    size_t offset;

    if (paint->v_ptr == paint->v_buf)
        return;

    glUniform1iv(glGetUniformLocation(paint->program, "u_Textures"), 4, array);

    for (int i = 0; i < paint->tex_count; i++)
        glBindTextureUnit(i, paint->tex_array[i]->id);

    glBindVertexArray(paint->vao);
    glBindBuffer(GL_ARRAY_BUFFER, paint->vbo);

    offset = paint->v_ptr - paint->v_buf;
    glBufferSubData(GL_ARRAY_BUFFER, 0, offset * sizeof(struct vertex), paint->v_buf);
    glDrawArrays(GL_TRIANGLES, 0, offset);

    paint->v_ptr = paint->v_buf;

    for (int i = 0; i < paint->tex_count; i++)
        paint->tex_array[i]->index = -1;
}

static void __GL_painter_push_vertices(struct painter *paint, struct vertex *vertices, size_t count)
{
    size_t offset;

    offset = paint->v_ptr - paint->v_buf;
    if ((offset + count) * sizeof(struct vertex) > VERTEX_BUFFER_LENGTH)
        GL_painter_end_frame(paint);

    memcpy(paint->v_ptr, vertices, count * sizeof(struct vertex));
    paint->v_ptr += count;
}

int GL_painter_create(struct painter *paint)
{
    struct image white;

    paint->v_buf = malloc(VERTEX_BUFFER_LENGTH);
    if (!paint->v_buf) {
        printf(ERROR "Failed to allocate vertex buffer\n");
        return -1;
    }

    glGenVertexArrays(1, &paint->vao);
    glBindVertexArray(paint->vao);

    glGenBuffers(1, &paint->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, paint->vbo);
    glBufferData(GL_ARRAY_BUFFER, VERTEX_BUFFER_LENGTH, NULL, GL_DYNAMIC_DRAW);

    /* position */
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
        sizeof(struct vertex), (void *)offsetof(struct vertex, position));
    glEnableVertexAttribArray(0);

    /* color */
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
        sizeof(struct vertex), (void *)offsetof(struct vertex, color));
    glEnableVertexAttribArray(1);

    /* center */
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
        sizeof(struct vertex), (void *)offsetof(struct vertex, center));
    glEnableVertexAttribArray(2);

    /* size */
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE,
        sizeof(struct vertex), (void *)offsetof(struct vertex, size));
    glEnableVertexAttribArray(3);

    /* radius */
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE,
        sizeof(struct vertex), (void *)offsetof(struct vertex, radius));
    glEnableVertexAttribArray(4);

    /* texture coords */
    glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE,
        sizeof(struct vertex), (void *)offsetof(struct vertex, texture_coords));
    glEnableVertexAttribArray(5);

    /* texture index */
    glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE,
        sizeof(struct vertex), (void *)offsetof(struct vertex, texture_index));
    glEnableVertexAttribArray(6);

    /* stroke width */
    glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE,
        sizeof(struct vertex), (void *)offsetof(struct vertex, stroke_width));
    glEnableVertexAttribArray(7);

    paint->program = __GL_create_shader(paint, PROJECT_DIR "/resources/vertex_2d.glsl", PROJECT_DIR "/resources/fragment_2d.glsl");
    if (!paint->program) {
        free(paint->v_buf);
        return -1;
    }

    white.data = (uint8_t[]){0xff, 0xff, 0xff, 0xff};
    white.width = 1;
    white.height = 1;
    __GL_texture_create(&paint->white_texture, &white);

    memset(paint->tex_array, 0, sizeof(paint->tex_array));
    paint->tex_count = 0;

    return 0;
}

void GL_painter_destroy(struct painter *paint)
{
    glDeleteBuffers(1, &paint->vbo);
    glDeleteVertexArrays(1, &paint->vao);
    glDeleteProgram(paint->program);

    __GL_texture_destroy(&paint->white_texture);
    free(paint->v_buf);
}

#define __COPY_COLOR_CENTER_SIZE_RADIUS_TEXTURE_WIDTH(i) \
    glm_vec4_copy(color, vertex[i].color);               \
    glm_vec2_copy(center, vertex[i].center);             \
    glm_vec2_copy(rect->size, vertex[i].size);           \
    glm_vec4_copy(radius, vertex[i].radius);             \
    vertex[i].texture_index = texture->index;            \
    vertex[i].stroke_width = stroke;

void GL_painter_draw_rect(struct painter *paint, struct rect *rect, struct style *style)
{
    struct vertex vertex[6];
    struct texture *texture;
    vec4 color, radius, tex_coords;
    vec2 center, half;
    float m, stroke;

    if (COLOR_A(style->color) == 0)
        return;

    if (style->style == STYLE_STROKE && style->st_width == 0.0f)
        return;

    glm_vec2_scale(rect->size, 0.5f, half);
    glm_vec2_add(rect->position, half, center);
    color_to_4f(style->color, color);

    m = min(half[0], half[1]);
    radius[0] = min(m, rect->radius[0]);
    radius[1] = min(m, rect->radius[1]);
    radius[2] = min(m, rect->radius[2]);
    radius[3] = min(m, rect->radius[3]);

    texture = style->style == STYLE_TEXTURED_FILL ? style->tx_texture : &paint->white_texture;
    __GL_painter_use_texture(paint, texture);

    tex_coords[0] = style->tx_coords[0] / texture->width;
    tex_coords[1] = style->tx_coords[1] / texture->height;
    tex_coords[2] = style->tx_coords[2] / texture->width + tex_coords[0];
    tex_coords[3] = style->tx_coords[3] / texture->height + tex_coords[1];

    stroke = style->style == STYLE_STROKE ? style->st_width : 0.0f;

    /* top right */
    glm_vec2_copy((vec2){rect->position[0] + rect->size[0], rect->position[1]}, vertex[0].position);
    __COPY_COLOR_CENTER_SIZE_RADIUS_TEXTURE_WIDTH(0);
    glm_vec2_copy((vec2){tex_coords[2], tex_coords[3]}, vertex[0].texture_coords);

    /* bottom right */
    glm_vec2_copy((vec2){rect->position[0] + rect->size[0], rect->position[1] + rect->size[1]}, vertex[1].position);
    __COPY_COLOR_CENTER_SIZE_RADIUS_TEXTURE_WIDTH(1);
    glm_vec2_copy((vec2){tex_coords[2], tex_coords[1]}, vertex[1].texture_coords);

    /* top left */
    glm_vec2_copy((vec2){rect->position[0], rect->position[1]}, vertex[2].position);
    __COPY_COLOR_CENTER_SIZE_RADIUS_TEXTURE_WIDTH(2);
    glm_vec2_copy((vec2){tex_coords[0], tex_coords[3]}, vertex[2].texture_coords);

    /* bottom right */
    glm_vec2_copy((vec2){rect->position[0] + rect->size[0], rect->position[1] + rect->size[1]}, vertex[3].position);
    __COPY_COLOR_CENTER_SIZE_RADIUS_TEXTURE_WIDTH(3);
    glm_vec2_copy((vec2){tex_coords[2], tex_coords[1]}, vertex[3].texture_coords);

    /* bottom left */
    glm_vec2_copy((vec2){rect->position[0], rect->position[1] + rect->size[1]}, vertex[4].position);
    __COPY_COLOR_CENTER_SIZE_RADIUS_TEXTURE_WIDTH(4);
    glm_vec2_copy((vec2){tex_coords[0], tex_coords[1]}, vertex[4].texture_coords);

    /* top left */
    glm_vec2_copy((vec2){rect->position[0], rect->position[1]}, vertex[5].position);
    __COPY_COLOR_CENTER_SIZE_RADIUS_TEXTURE_WIDTH(5);
    glm_vec2_copy((vec2){tex_coords[0], tex_coords[3]}, vertex[5].texture_coords);

    __GL_painter_push_vertices(paint, vertex, ARRAY_SIZE(vertex));
}
