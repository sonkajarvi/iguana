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
#include <iguana/div.h>
#include <iguana/widget.h>
#include <iguana/window.h>

#include "renderer.h"

static void div_relayout(struct igu_widget *wgt)
{
    struct igu_div *div = igu_container_of(wgt, struct igu_div, widget);
    struct igu_widget *tmp, *end;
    igu_rect_t area;
    uint32_t start, offset;

    if (div->child_count == 0)
        return;

    igu_rect_with_margin_padding(wgt, area);

    if (div->direction == IGU_DIV_COLUMN) {
        start = IGU_X(area);
        offset = IGU_WIDTH(area) / div->child_count;
    } else {
        start = IGU_Y(area);
        offset = IGU_HEIGHT(area) / div->child_count;
    }

    for (tmp = div->child, end = tmp; tmp && tmp->next != end; tmp = tmp->next) {
        /* TODO: do this elsewhere, or get it some other way */
        tmp->window = div->widget.window;

        if (div->direction == IGU_DIV_COLUMN) {
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

        WIDGET_RELAYOUT(tmp);
        start += offset;
    }
}

static void div_repaint(struct igu_widget *wgt)
{
    struct igu_div *div = igu_container_of(wgt, struct igu_div, widget);
    struct igu_widget *tmp, *end;
    struct rect rect;
    struct style style;
    igu_rect_t area;

    igu_rect_with_margin(wgt, area);

    memset(&rect, 0, sizeof(rect));
    rect.position[0] = IGU_X(area);
    rect.position[1] = IGU_Y(area);
    rect.size[0] = IGU_WIDTH(area);
    rect.size[1] = IGU_HEIGHT(area);

    memset(&style, 0, sizeof(style));
    style.type = STYLE_FILL;
    style.color = div->background_color;

    renderer_draw_rect(wgt->window->renderer, &rect, &style);

    if (div->child_count == 0)
        return;

    for (tmp = div->child, end = tmp; tmp && tmp->next != end; tmp = tmp->next) {
        WIDGET_REPAINT(tmp);
    }
}

static void div_free(struct igu_widget *wgt)
{
    struct igu_div *div = igu_container_of(wgt, struct igu_div, widget);
    struct igu_widget *tmp;

    for (tmp = div->child; tmp; tmp = div->child) {
        igu_div_del_child(div, tmp);
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

#define PRINT_VALUE(value_)                                                      \
    do {                                                                         \
        printf("%u", (value_).value);                                            \
        if ((value_).value)                                                      \
            printf("%s", (value_).type == IGU_VALUE_TYPE_ABSOLUTE ? "px" : "%"); \
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

static void div_print(struct igu_widget *wgt, int depth)
{
    struct igu_div *div = igu_container_of(wgt, struct igu_div, widget);
    struct igu_widget *tmp, *end;
    const char *color;
    igu_rect_t m_rect, mp_rect;

    igu_rect_with_margin(wgt, m_rect);
    igu_rect_with_margin_padding(wgt, mp_rect);
    color = igu_color_to_string(div->background_color);

    PRINT       ("div\n");
    PRINT       ("  this: %p\n", div);
    PRINT       ("  parent: %p\n", wgt->parent);
    PRINT       ("  area: %u %u %u %u\n", IGU_X(wgt->rect), IGU_Y(wgt->rect),
                 IGU_WIDTH(wgt->rect), IGU_HEIGHT(wgt->rect));
    PRINT       ("        %u %u %u %u\n", IGU_X(m_rect), IGU_Y(m_rect),
                 IGU_WIDTH(m_rect), IGU_HEIGHT(m_rect));
    PRINT       ("        %u %u %u %u\n", IGU_X(mp_rect), IGU_Y(mp_rect),
                 IGU_WIDTH(mp_rect), IGU_HEIGHT(mp_rect));
    PRINT       ("  name: %s\n", wgt->name);
    PRINT       ("  color: #%08x %s\n", div->background_color, color ?: "");
    PRINT_VALUES("  margin:", wgt->margin);
    PRINT_VALUES("  padding:", wgt->padding);
    PRINT       ("  direction: %s\n", div->direction == IGU_DIV_ROW ? "row" :
                 "column");
    PRINT       ("  children: [%u]\n", div->child_count);

    for (tmp = div->child, end = tmp; tmp && tmp->next != end; tmp = tmp->next)
        WIDGET_PRINT_N(tmp, depth + 1);
}

static const struct igu_widget_vtable div_vtable = {
    .relayout = div_relayout,
    .repaint = div_repaint,
    .free = div_free,
    .print = div_print,
};

static const struct igu_div div_default = {
    .widget = {
        .vtable = &div_vtable,
    },
    .direction = IGU_DIV_COLUMN,
    .background_color = IGU_TRANSPARENT,
};

struct igu_div *igu_div_new(void)
{
    struct igu_div *div = malloc(sizeof(*div));
    if (!div)
        return NULL;

    memcpy(div, &div_default, sizeof(*div));
    return div;
}

void igu_div_add_child(struct igu_div *div, struct igu_widget *wgt)
{
    struct igu_widget *tmp = div->child, *end = tmp;

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

void igu_div_del_child(struct igu_div *div, struct igu_widget *wgt)
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
