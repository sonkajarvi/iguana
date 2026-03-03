/**
 * Copyright (c) 2026, sonkajarvi
 *
 * Licensed under the BSD 2-Clause License.
 * The full license can be found in the LICENSE.txt file.
 */

#ifndef __VALUE_H
#define __VALUE_H

#include <stdint.h>

#include <gui/accessor.h>

/*
 * VALUE_TYPE_RELATIVE - Relative to width/height.
 * VALUE_TYPE_ABSOLUTE - Screen pixels.
 */
#define VALUE_TYPE_ABSOLUTE 0
#define VALUE_TYPE_RELATIVE 1

#define PX(x)                        \
    ((value_t){                      \
        .type = VALUE_TYPE_ABSOLUTE, \
        .value = (x),                \
    })

#define PER(x)                       \
    ((value_t){                      \
        .type = VALUE_TYPE_RELATIVE, \
        .value = (x),                \
    })

typedef struct {
    uint8_t type;
    uint32_t value;
} value_t;

/* TODO: This is 32 bytes, and it can (and should) be optimized down. */
typedef value_t value4_t[4];

/*
 * There are three ways to set a four-component value. Below are two helper
 * functions that can be used for the two first ways. For the last one, just
 * set the components by directly accessing the array.
 *
 *      a            -->     a a a a
 *      a b          -->     a b a b
 *      a b c d      -->     a b c d
 */

static inline void value_set_all(value4_t value, value_t all)
{
    TOP(value) = all;
    RIGHT(value) = all;
    BOTTOM(value) = all;
    LEFT(value) = all;
}

static inline void value_set_axis(value4_t value, value_t vert, value_t horiz)
{
    TOP(value) = vert;
    RIGHT(value) = horiz;
    BOTTOM(value) = vert;
    LEFT(value) = horiz;
}

static inline uint32_t value(value_t value, uint32_t rel)
{
    switch (value.type) {
    case VALUE_TYPE_ABSOLUTE:
        return value.value;

    case VALUE_TYPE_RELATIVE:
        return rel / 100 * value.value;

    default:
        return -1;
    }
}

#endif /* !__VALUE_H */
