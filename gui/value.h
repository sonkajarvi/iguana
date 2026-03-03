/**
 * Copyright (c) 2026, sonkajarvi
 *
 * Licensed under the BSD 2-Clause License.
 * The full license can be found in the LICENSE.txt file.
 */

#ifndef __VALUE_H
#define __VALUE_H

#include <stdint.h>

/*
 * There are three ways to set a four-component value. Below are two helper
 * functions that can be used for the two first ways. For the last one, just
 * set the components by directly accessing the array.
 *
 *      a            -->     a a a a
 *      a b          -->     a b a b
 *      a b c d      -->     a b c d
 */

static inline void value_set_4(uint32_t val[static 4], uint32_t a)
{
    val[0] = a;
    val[1] = a;
    val[2] = a;
    val[3] = a;
}

static inline void value_set_2(uint32_t val[static 4], uint32_t a, uint32_t b)
{
    val[0] = a;
    val[1] = b;
    val[2] = a;
    val[3] = b;
}

#endif /* !__VALUE_H */
