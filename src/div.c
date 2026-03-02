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
    uint32_t x, width;

    if (div->child_count == 0)
        return;

    x = wgt->x;
    width = wgt->width / div->child_count;

    for (tmp = div->child, end = tmp; tmp && tmp->next != end; tmp = tmp->next) {
        /* TODO: do this elsewhere, or get it some other way */
        tmp->window = div->widget.window;

        tmp->x = x;
        tmp->y = wgt->y;
        tmp->width = width;
        tmp->height = wgt->height;
        WIDGET_RELAYOUT(tmp);

        x += width;
    }
}

static void div_repaint(struct widget *wgt)
{
    struct div *div = __container_of(wgt, struct div, widget);
    struct widget *tmp, *end;
    struct rect rect;
    struct style style;

    memset(&rect, 0, sizeof(rect));
    rect.position[0] = wgt->x;
    rect.position[1] = wgt->y;
    rect.size[0] = (float)wgt->width;
    rect.size[1] = (float)wgt->height;

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

static void div_print(struct widget *wgt)
{
    struct div *div = __container_of(wgt, struct div, widget);
    struct widget *tmp, *end;

    printf("div name=\"%s\" this=%p parent=%p children=%u color=%08x rect=%u,%u,%u,%u\n",
        wgt->name, div, wgt->parent, div->child_count, div->background_color,
        wgt->x, wgt->y, wgt->width, wgt->height);

    for (tmp = div->child, end = tmp; tmp && tmp->next != end; tmp = tmp->next) {
        putchar('\t');
        WIDGET_PRINT(tmp);
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
