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
 * @IGU_VALUE_ABSOLUTE - Absolute value in screen pixels
 * @IGU_VALUE_RELATIVE - Relative to parent width or height
 */
enum {
    IGU_VALUE_ABSOLUTE = 0,
    IGU_VALUE_RELATIVE,
};

typedef struct {
    int type;
    float value;
} igu_value_t;

typedef igu_value_t igu_value4_t[4];

#define IGU_PX(x)                       \
    ((igu_value_t){                     \
        .type = IGU_VALUE_ABSOLUTE,     \
        .value = (x),                   \
    })

#define IGU_PCT(x)                      \
    ((igu_value_t){                     \
        .type = IGU_VALUE_RELATIVE,     \
        .value = (x),                   \
    })

/*
 * There are three ways to set a four-component value. Below are two helper
 * functions that can be used for the two first ways. For the last one, just
 * set the components by directly accessing the array.
 *
 *      a            -->     a a a a
 *      a b          -->     a b a b
 *      a b c d      -->     a b c d
 */

static inline void igu_value_set_one(igu_value4_t value, igu_value_t xyzw)
{
    IGU_X(value) = xyzw;
    IGU_Y(value) = xyzw;
    IGU_Z(value) = xyzw;
    IGU_W(value) = xyzw;
}

static inline void igu_value_set_two(igu_value4_t value, igu_value_t xz,
                                     igu_value_t yw)
{
    IGU_X(value) = xz;
    IGU_Y(value) = yw;
    IGU_Z(value) = xz;
    IGU_W(value) = yw;
}

static inline void igu_value_set_four(igu_value4_t value, igu_value_t x,
                                      igu_value_t y, igu_value_t z,
                                      igu_value_t w)
{
    IGU_X(value) = x;
    IGU_Y(value) = y;
    IGU_Z(value) = z;
    IGU_W(value) = w;
}

static inline uint32_t igu_value(igu_value_t value, float relative)
{
    switch (value.type) {
    case IGU_VALUE_ABSOLUTE:
        return value.value;

    case IGU_VALUE_RELATIVE:
        return relative * value.value / 100.0f;

    default:
        return -1;
    }
}

#endif /* !_IGUANA_VALUE_H */
