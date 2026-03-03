/**
 * Copyright (c) 2025, sonkajarvi
 *
 * Licensed under the BSD 2-Clause License.
 * The full license can be found in the LICENSE.txt file.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gui/color.h>
#include <gui/div.h>
#include <gui/widget.h>
#include <gui/window.h>

#include "internal.h"

static void div_relayout(struct widget *wgt)
{
    struct div *div = __container_of(wgt, struct div, widget);
    struct widget *tmp, *end;
    rect_t area;
    uint32_t start, offset;

    if (div->child_count == 0)
        return;

    rect_with_margin_padding(wgt, area);

    if (div->direction == DIV_COLUMN) {
        start = X(area);
        offset = WIDTH(area) / div->child_count;
    } else {
        start = Y(area);
        offset = HEIGHT(area) / div->child_count;
    }

    for (tmp = div->child, end = tmp; tmp && tmp->next != end; tmp = tmp->next) {
        /* TODO: do this elsewhere, or get it some other way */
        tmp->window = div->widget.window;

        if (div->direction == DIV_COLUMN) {
            X(tmp->rect) = start;
            Y(tmp->rect) = Y(area);
            WIDTH(tmp->rect) = offset;
            HEIGHT(tmp->rect) = HEIGHT(area);
        } else {
            X(tmp->rect) = X(area);
            Y(tmp->rect) = start;
            WIDTH(tmp->rect) = WIDTH(area);
            HEIGHT(tmp->rect) = offset;
        }

        WIDGET_RELAYOUT(tmp);
        start += offset;
    }
}

static void div_repaint(struct widget *wgt)
{
    struct div *div = __container_of(wgt, struct div, widget);
    struct widget *tmp, *end;
    struct rect rect;
    struct style style;
    rect_t area;

    rect_with_margin(wgt, area);

    memset(&rect, 0, sizeof(rect));
    rect.position[0] = X(area);
    rect.position[1] = Y(area);
    rect.size[0] = WIDTH(area);
    rect.size[1] = HEIGHT(area);

    memset(&style, 0, sizeof(style));
    style.style = STYLE_FILL;
    style.color = div->background_color;

    GL_painter_draw_rect(&wgt->window->painter, &rect, &style);

    if (div->child_count == 0)
        return;

    for (tmp = div->child, end = tmp; tmp && tmp->next != end; tmp = tmp->next) {
        WIDGET_REPAINT(tmp);
    }
}

static void div_free(struct widget *wgt)
{
    struct div *div = __container_of(wgt, struct div, widget);
    struct widget *tmp;

    for (tmp = div->child; tmp; tmp = div->child) {
        div_del_child(div, tmp);
        WIDGET_FREE(tmp);
    }

    free(div);
}

#define INDENT()                            \
    do {                                    \
        for (int i = 0; i < depth * 4; i++) \
            putchar(' ');                   \
    } while (0)

#define PRINT(fmt, ...)                     \
    do {                                    \
        INDENT();                           \
        printf(fmt, ##__VA_ARGS__);         \
    } while (0)

#define PRINT_VALUE(value_)                                                   \
    do {                                                                      \
        printf("%u", (value_).value);                                         \
        if ((value_).value)                                                   \
            printf("%s", (value_).type == VALUE_TYPE_ABSOLUTE ? "px" : "%"); \
    } while (0)

#define PRINT_VALUES(fmt, values)                  \
    do {                                           \
        INDENT();                                  \
        printf(fmt);                               \
        putchar(' '); PRINT_VALUE(TOP(values));    \
        putchar(' '); PRINT_VALUE(RIGHT(values));  \
        putchar(' '); PRINT_VALUE(BOTTOM(values)); \
        putchar(' '); PRINT_VALUE(LEFT(values));   \
        putchar('\n');                             \
    } while (0)

static void div_print(struct widget *wgt, int depth)
{
    struct div *div = __container_of(wgt, struct div, widget);
    struct widget *tmp, *end;
    const char *color;
    rect_t m_rect, mp_rect;

    rect_with_margin(wgt, m_rect);
    rect_with_margin_padding(wgt, mp_rect);
    color = color_to_string(div->background_color);

    PRINT       ("div\n");
    PRINT       ("  this: %p\n", div);
    PRINT       ("  parent: %p\n", wgt->parent);
    PRINT       ("  area: %u %u %u %u\n", X(wgt->rect), Y(wgt->rect), WIDTH(wgt->rect), HEIGHT(wgt->rect));
    PRINT       ("        %u %u %u %u\n", X(m_rect), Y(m_rect), WIDTH(m_rect), HEIGHT(m_rect));
    PRINT       ("        %u %u %u %u\n", X(mp_rect), Y(mp_rect), WIDTH(mp_rect), HEIGHT(mp_rect));
    PRINT       ("  name: %s\n", wgt->name);
    PRINT       ("  color: #%08x %s\n", div->background_color, color ?: "");
    PRINT_VALUES("  margin:", wgt->margin);
    PRINT_VALUES("  padding:", wgt->padding);
    PRINT       ("  direction: %s\n", div->direction == DIV_ROW ? "row" : "column");
    PRINT       ("  children: [%u]\n", div->child_count);

    for (tmp = div->child, end = tmp; tmp && tmp->next != end; tmp = tmp->next) {
        WIDGET_PRINT_N(tmp, depth + 1);
    }
}

static const struct widget_vtable div_vtable = {
    .relayout = div_relayout,
    .repaint = div_repaint,
    .free = div_free,
    .print = div_print,
};

static const struct div div_default = {
    .widget = {
        .vtable = &div_vtable,
    },
    .direction = DIV_COLUMN,
    .background_color = COLOR_TRANSPARENT,
};

struct div *div_new(void)
{
    struct div *div = malloc(sizeof(*div));
    if (!div)
        return NULL;

    memcpy(div, &div_default, sizeof(*div));
    return div;
}

void div_add_child(struct div *div, struct widget *wgt)
{
    struct widget *tmp = div->child, *end = tmp;

    if (!tmp) {
        div->child = wgt;
    } else {
        while (tmp && tmp != end)
            tmp = tmp->next;

        wgt->next = tmp->next;
        wgt->prev = tmp;
        tmp->next = wgt;
    }

    wgt->parent = &div->widget;
    div->child_count++;
}

void div_del_child(struct div *div, struct widget *wgt)
{
    if (div->child == wgt)
        div->child = wgt->next;

    if (wgt->prev)
        wgt->prev->next = wgt->next;
    if (wgt->next)
        wgt->next->prev = wgt->prev;

    wgt->prev = NULL;
    wgt->next = NULL;
    wgt->parent = NULL;
    div->child_count--;
}
