/**
 * Copyright (c) 2026, sonkajarvi
 *
 * Licensed under the BSD 2-Clause License.
 * The full license can be found in the LICENSE.txt file.
 */

#ifndef _IGUANA_VALUE_H
#define _IGUANA_VALUE_H

#include <stdint.h>

#include <iguana/accessor.h>

/*
 * @IGU_VALUE_TYPE_RELATIVE - Relative to width/height.
 * @IGU_VALUE_TYPE_ABSOLUTE - Screen pixels.
 */
#define IGU_VALUE_TYPE_ABSOLUTE 0
#define IGU_VALUE_TYPE_RELATIVE 1

#define IGU_PX(x)                           \
    ((igu_value_t){                         \
        .type = IGU_VALUE_TYPE_ABSOLUTE,    \
        .value = (x),                       \
    })

#define IGU_PER(x)                          \
    ((igu_value_t){                         \
        .type = IGU_VALUE_TYPE_RELATIVE,    \
        .value = (x),                       \
    })

typedef struct {
    uint8_t type;
    uint32_t value;
} igu_value_t;

/* TODO: This is 32 bytes, and it can (and should) be optimized down. */
typedef igu_value_t igu_value4_t[4];

/*
 * There are three ways to set a four-component value. Below are two helper
 * functions that can be used for the two first ways. For the last one, just
 * set the components by directly accessing the array.
 *
 *      a            -->     a a a a
 *      a b          -->     a b a b
 *      a b c d      -->     a b c d
 */

static inline void igu_value_set_all(igu_value4_t value, igu_value_t all)
{
    IGU_TOP(value) = all;
    IGU_RIGHT(value) = all;
    IGU_BOTTOM(value) = all;
    IGU_LEFT(value) = all;
}

static inline void igu_value_set_axis(igu_value4_t value, igu_value_t vert,
                                      igu_value_t horiz)
{
    IGU_TOP(value) = vert;
    IGU_RIGHT(value) = horiz;
    IGU_BOTTOM(value) = vert;
    IGU_LEFT(value) = horiz;
}

static inline uint32_t igu_value(igu_value_t value, uint32_t rel)
{
    switch (value.type) {
    case IGU_VALUE_TYPE_ABSOLUTE:
        return value.value;

    case IGU_VALUE_TYPE_RELATIVE:
        return rel * value.value / 100;

    default:
        return -1;
    }
}

#endif /* !_IGUANA_VALUE_H */
