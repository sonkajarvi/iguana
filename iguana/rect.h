/**
 * Copyright (c) 2026, sonkajarvi
 *
 * Licensed under the BSD 2-Clause License.
 * The full license can be found in the LICENSE.txt file.
 */

#ifndef _IGUANA_RECT_H
#define _IGUANA_RECT_H

#include <stdbool.h>

#include <iguana/accessor.h>

/* For printing */
#define IGU_RECT_FMT "[%.1f %.1f %.1f %.1f]"
#define IGU_RECT_PRINT(x) IGU_X(x), IGU_Y(x), IGU_Z(x), IGU_W(x)

typedef float igu_rect_t[4];

static inline bool igu_rect_point_in_rect(igu_rect_t rect, float x, float y)
{
    return x > IGU_X(rect) && x < IGU_X(rect) + IGU_WIDTH(rect) &&
           y > IGU_Y(rect) && y < IGU_Y(rect) + IGU_HEIGHT(rect);
}

#endif /* !_IGUANA_RECT_H */
