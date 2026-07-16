/**
 * Copyright (c) 2025-2026, sonkajarvi
 *
 * Licensed under the BSD 2-Clause License.
 * The full license can be found in the LICENSE.txt file.
 */

#ifndef _IGUANA_DIV_H
#define _IGUANA_DIV_H

#include <stdint.h>

#include <iguana/widget.h>

#define IGU_DIV_COLUMN      0
#define IGU_DIV_ROW         1

struct igu_div {
    struct igu_widget widget;

    struct igu_widget *child;
    uint32_t child_count;

    int direction;

    uint32_t background_color;
};

struct igu_div *igu_div_new(void);
void igu_div_add_child(struct igu_div *div, struct igu_widget *wgt);
void igu_div_del_child(struct igu_div *div, struct igu_widget *wgt);

#endif /* !_IGUANA_DIV_H */
