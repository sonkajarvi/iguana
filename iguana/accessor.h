/**
 * Copyright (c) 2026, sonkajarvi
 *
 * Licensed under the BSD 2-Clause License.
 * The full license can be found in the LICENSE.txt file.
 */

#ifndef _IGUANA_ACCESSOR_H
#define _IGUANA_ACCESSOR_H

#define IGU_X(x)            ((x)[0])
#define IGU_Y(x)            ((x)[1])
#define IGU_Z(x)            ((x)[2])
#define IGU_W(x)            ((x)[3])

#define IGU_WIDTH(x)        IGU_Z(x)
#define IGU_HEIGHT(x)       IGU_W(x)

#define IGU_TOP(x)          ((x)[0])
#define IGU_RIGHT(x)        ((x)[1])
#define IGU_BOTTOM(x)       ((x)[2])
#define IGU_LEFT(x)         ((x)[3])

#endif /* !_IGUANA_ACCESSOR_H */
