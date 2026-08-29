/**
 * Copyright (c) 2026, sonkajarvi
 *
 * Licensed under the BSD 2-Clause License.
 * The full license can be found in the LICENSE.txt file.
 */

#ifndef _SRC_DEBUG_H
#define _SRC_DEBUG_H

#include <stddef.h>
#include <stdint.h>

#include <iguana/value.h>

void igu_debug_print_label(size_t depth, const char *label);

void igu_debug_print_pointer(size_t depth, const char *label, const void *ptr);
void igu_debug_print_string(size_t depth, const char *label, const char *str);
void igu_debug_print_color(size_t depth, const char *label, uint32_t color);
void igu_debug_print_value4(size_t depth, const char *label,
                            const igu_value4_t *value);
void igu_debug_print_children(size_t depth, const char *label, size_t count);

#endif /* !_SRC_DEBUG_H */
