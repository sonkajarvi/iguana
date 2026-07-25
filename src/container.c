/**
 * Copyright (c) 2025-2026, sonkajarvi
 *
 * Licensed under the BSD 2-Clause License.
 * The full license can be found in the LICENSE.txt file.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iguana/color.h>
#include <iguana/container.h>
#include <iguana/rect.h>
#include <iguana/widget.h>
#include <iguana/window.h>

#include "renderer.h"

#define container_for_each_child(_container, _child)                          \
    for (struct igu_widget *_child = (_container)->children, *__end = _child; \
         _child && _child->next != __end;                                     \
         _child = _child->next)

static void container_relayout(struct igu_widget *widget)
{
    struct igu_container *container = igu_container_of(widget,
                                                       struct igu_container,
                                                       widget);
    struct igu_widget *tmp, *end;
    igu_rect_t area;
    uint32_t start, offset;

    if (container->widget.num_children == 0)
        return;

    igu_widget_rect_with_margin_and_padding(widget, area);

    if (container->direction == IGU_CONTAINER_VERTICAL) {
        start = IGU_X(area);
        offset = IGU_WIDTH(area) / container->widget.num_children;
    } else {
        start = IGU_Y(area);
        offset = IGU_HEIGHT(area) / container->widget.num_children;
    }

    for (tmp = container->widget.children, end = tmp;
         tmp && tmp->next != end; tmp = tmp->next) {
        /* TODO: do this elsewhere, or get it some other way */
        tmp->window = container->widget.window;

        if (container->direction == IGU_CONTAINER_VERTICAL) {
            IGU_X(tmp->rect) = start;
            IGU_Y(tmp->rect) = IGU_Y(area);
            IGU_WIDTH(tmp->rect) = offset;
            IGU_HEIGHT(tmp->rect) = IGU_HEIGHT(area);
        } else {
            IGU_X(tmp->rect) = IGU_X(area);
            IGU_Y(tmp->rect) = start;
            IGU_WIDTH(tmp->rect) = IGU_WIDTH(area);
            IGU_HEIGHT(tmp->rect) = offset;
        }

        igu_widget_relayout(tmp);
        start += offset;
    }
}

static void container_repaint(struct igu_widget *widget)
{
    struct igu_container *container = igu_container_of(widget,
                                                       struct igu_container,
                                                       widget);
    struct igu_widget *tmp, *end;
    struct rect rect;
    struct style style;
    igu_rect_t area;

    igu_widget_rect_with_margin(widget, area);

    memset(&rect, 0, sizeof(rect));
    rect.position[0] = IGU_X(area);
    rect.position[1] = IGU_Y(area);
    rect.size[0] = IGU_WIDTH(area);
    rect.size[1] = IGU_HEIGHT(area);

    memset(&style, 0, sizeof(style));
    style.type = STYLE_FILL;
    style.color = container->background_color;

    renderer_draw_rect(widget->window->renderer, &rect, &style);

    if (container->border_width > 0.0f) {
        style.type = STYLE_STROKE;
        style.color = container->border_color;
        style.stroke_width = container->border_width;

        renderer_draw_rect(widget->window->renderer, &rect, &style);
    }

    if (container->widget.num_children == 0)
        return;

    for (tmp = container->widget.children, end = tmp;
         tmp && tmp->next != end; tmp = tmp->next) {
        igu_widget_redraw(tmp);
    }
}

static void container_free(struct igu_widget *widget)
{
    struct igu_container *container = igu_container_of(widget,
                                                       struct igu_container,
                                                       widget);
    struct igu_widget *tmp = container->widget.children;

    while (tmp) {
        igu_container_del_child(container, tmp);
        igu_widget_free(tmp);

        tmp = container->widget.children;
    }

    free(container);
}

#define INDENT()                                \
    do {                                        \
        for (int i = 0; i < depth * 8; i++)     \
            putchar(' ');                       \
    } while (0)

#define PRINT(fmt, ...)                 \
    do {                                \
        INDENT();                       \
        printf(fmt, ##__VA_ARGS__);     \
    } while (0)

#define PRINT_VALUE(value_)                                                 \
    do {                                                                    \
        printf("%.1f", (value_).value);                                     \
        if ((value_).value)                                                 \
            printf("%s", (value_).type == IGU_VALUE_ABSOLUTE ? "px" : "%"); \
    } while (0)

#define PRINT_VALUES(fmt, values)                       \
    do {                                                \
        INDENT();                                       \
        printf(fmt);                                    \
        putchar(' '); PRINT_VALUE(IGU_TOP(values));     \
        putchar(' '); PRINT_VALUE(IGU_RIGHT(values));   \
        putchar(' '); PRINT_VALUE(IGU_BOTTOM(values));  \
        putchar(' '); PRINT_VALUE(IGU_LEFT(values));    \
        putchar('\n');                                  \
    } while (0)

#define ARGB(x)                 \
    (IGU_COLOR_A(x) << 24 |     \
     IGU_COLOR_R(x) << 16 |     \
     IGU_COLOR_G(x) << 8  |     \
     IGU_COLOR_B(x))

static void container_print(struct igu_widget *widget, int depth)
{
    struct igu_container *container = igu_container_of(widget,
                                                       struct igu_container,
                                                       widget);
    struct igu_widget *tmp, *end;
    const char *color;
    igu_rect_t m_rect, mp_rect;

    igu_widget_rect_with_margin(widget, m_rect);
    igu_widget_rect_with_margin_and_padding(widget, mp_rect);
    color = igu_color_to_string(container->background_color) ?: "";

    PRINT       ("container:\n");
    PRINT       ("    this: %p\n", container);
    PRINT       ("    parent: %p\n", widget->parent);
    PRINT       ("    area: " IGU_RECT_FMT "\n", IGU_RECT_PRINT(widget->rect));
    PRINT       ("          " IGU_RECT_FMT "\n", IGU_RECT_PRINT(m_rect));
    PRINT       ("          " IGU_RECT_FMT "\n", IGU_RECT_PRINT(mp_rect));
    PRINT       ("    name: %s\n", widget->name);
    PRINT       ("    color: #%08x %s\n", ARGB(container->background_color),
                 color);
    PRINT_VALUES("    margin:", widget->margin);
    PRINT_VALUES("    padding:", widget->padding);
    PRINT       ("    direction: %s\n", container->direction ==
                 IGU_CONTAINER_HORIZONTAL ? "row" : "column");
    PRINT       ("    children: [%zu]\n", widget->num_children);

    for (tmp = widget->children, end = tmp;
         tmp && tmp->next != end; tmp = tmp->next)
        __igu_widget_print(tmp, depth + 1);
}

static const struct igu_widget_vtable container_vtable = {
    .relayout = container_relayout,
    .redraw = container_repaint,
    .free = container_free,
    .print = container_print,
};

static const struct igu_container container_default = {
    .widget = {
        .vtable = &container_vtable,
    },
    .background_color = IGU_TRANSPARENT,
    .border_color = IGU_TRANSPARENT,
    .border_width = 0.0f,
    .direction = IGU_CONTAINER_HORIZONTAL,
};

struct igu_container *igu_container_create(void)
{
    struct igu_container *container = malloc(sizeof(*container));
    if (!container)
        return NULL;

    memcpy(container, &container_default, sizeof(*container));
    return container;
}

void igu_container_add_child(struct igu_container *container,
                             struct igu_widget *widget)
{
    struct igu_widget *tmp = container->widget.children, *end = tmp;

    if (!tmp) {
        container->widget.children = widget;
    } else {
        while (tmp && tmp != end)
            tmp = tmp->next;

        widget->next = tmp->next;
        widget->prev = tmp;
        tmp->next = widget;
    }

    widget->parent = &container->widget;
    container->widget.num_children++;
}

void igu_container_del_child(struct igu_container *container,
                             struct igu_widget *widget)
{
    if (container->widget.children == widget)
        container->widget.children = widget->next;

    if (widget->prev)
        widget->prev->next = widget->next;
    if (widget->next)
        widget->next->prev = widget->prev;

    widget->prev = NULL;
    widget->next = NULL;
    widget->parent = NULL;
    container->widget.num_children--;
}
