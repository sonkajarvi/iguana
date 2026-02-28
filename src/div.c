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

static void __div_relayout(struct widget *wgt);
static void __div_free(struct widget *wgt);
static void __div_print(struct widget *wgt);

static struct div __div_default = {
    .widget = {
        .relayout = __div_relayout,
        .free = __div_free,
        .print = __div_print,
    },
    .background_color = COLOR_TRANSPARENT,
};

static void __div_relayout(struct widget *wgt)
{
    struct div *div = __container_of(wgt, struct div, widget);
    struct widget *tmp, *end;
    uint32_t x, width;

    if (div->child_count == 0)
        return;

    x = wgt->x;
    width = wgt->width / div->child_count;

    for (tmp = div->child, end = tmp; tmp && tmp->next != end; tmp = tmp->next) {
        tmp->x = x;
        tmp->y = wgt->y;
        tmp->width = width;
        tmp->height = wgt->height;
        WIDGET_RELAYOUT(tmp);

        x += width;
    }
}

static void __div_free(struct widget *wgt)
{
    struct div *div = __container_of(wgt, struct div, widget);
    struct widget *tmp;

    for (tmp = div->child; tmp; tmp = div->child) {
        div_del_child(div, tmp);
        WIDGET_FREE(tmp);
    }

    free(div);
}

static void __div_print(struct widget *wgt)
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

struct div *div_new(void)
{
    struct div *div = malloc(sizeof(*div));
    if (!div)
        return NULL;

    memcpy(div, &__div_default, sizeof(*div));
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
