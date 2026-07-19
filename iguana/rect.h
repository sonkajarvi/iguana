/**
 * Copyright (c) 2026, sonkajarvi
 *
 * Licensed under the BSD 2-Clause License.
 * The full license can be found in the LICENSE.txt file.
 */

#ifndef _IGUANA_RECT_H
#define _IGUANA_RECT_H

#include <iguana/accessor.h>

/* For printing */
#define IGU_RECT_FMT "[%.1f %.1f %.1f %.1f]"
#define IGU_RECT_PRINT(x) IGU_X(x), IGU_Y(x), IGU_Z(x), IGU_W(x)

typedef float igu_rect_t[4];

#endif /* !_IGUANA_RECT_H */
