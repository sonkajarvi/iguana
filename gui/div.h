/**
 * Copyright (c) 2025, sonkajarvi
 *
 * Licensed under the BSD 2-Clause License.
 * The full license can be found in the LICENSE.txt file.
 */

#ifndef __DIV_H
#define __DIV_H

#include <stdint.h>

#include <gui/widget.h>

#define DIV_COLUMN      0
#define DIV_ROW         1

struct div {
    struct widget widget;

    struct widget *child;
    uint32_t child_count;

    int direction;

    uint32_t background_color;
};

struct div *div_new(void);
void div_add_child(struct div *div, struct widget *wgt);
void div_del_child(struct div *div, struct widget *wgt);

#endif /* !__DIV_H */
