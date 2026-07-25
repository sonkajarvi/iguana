/**
 * Copyright (c) 2025-2026, sonkajarvi
 *
 * Licensed under the BSD 2-Clause License.
 * The full license can be found in the LICENSE.txt file.
 */

#ifndef _IGUANA_CONTAINER_H
#define _IGUANA_CONTAINER_H

#include <stddef.h>
#include <stdint.h>

#include <iguana/widget.h>

enum {
    /* Default to horizontal layout */
    IGU_CONTAINER_HORIZONTAL = 0,
    IGU_CONTAINER_VERTICAL,
};

/**
 * struct igu_container - A generic container for holding multiple widgets
 */
struct igu_container {
    struct igu_widget widget;

    /* styles */
    uint32_t background_color;
    uint32_t border_color;
    float border_width;
    int direction;
};

struct igu_container *igu_container_create(void);

void igu_container_add_child(struct igu_container *container,
                             struct igu_widget *widget);
void igu_container_del_child(struct igu_container *container,
                             struct igu_widget *widget);

#endif /* !_IGUANA_CONTAINER_H */
