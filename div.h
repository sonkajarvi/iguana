/**
 * Copyright (c) 2025, sonkajarvi
 *
 * Licensed under the BSD 2-Clause License.
 * The full license can be found in the LICENSE.txt file.
 */

#ifndef __DIV_H__
#define __DIV_H__

#include <cglm/vec4.h>

#include "gui.h"

struct div {
    DEFINE_WIDGET();

    vec4 background_color;
};

void div_create(struct div *div);

#endif /* !__DIV_H__ */
