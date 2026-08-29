/**
 * Copyright (c) 2026, sonkajarvi
 *
 * Licensed under the BSD 2-Clause License.
 * The full license can be found in the LICENSE.txt file.
 */

#include <stdio.h>

#include <iguana/accessor.h>
#include <iguana/color.h>
#include <iguana/value.h>

#include "debug.h"

#define IGU_TERM_NONE       "\033[0m"
#define IGU_TERM_STR        "\033[92m"
#define IGU_TERM_COLOR      "\033[93m"
#define IGU_TERM_PTR        "\033[95m"
#define IGU_TERM_VALUE      "\033[96m"
#define IGU_TERM_COMMENT    "\033[94m"

#define IGU_DEBUG_PRINT_INDENT_STRING "    "

static inline void igu_debug_print_indent(size_t depth)
{
    for (size_t i = 0; i < depth; i++)
        printf("%s", IGU_DEBUG_PRINT_INDENT_STRING);
}

void igu_debug_print_label(size_t depth, const char *label)
{
    igu_debug_print_indent(depth);
    printf("%s:\n", label);
}

void igu_debug_print_pointer(size_t depth, const char *label, const void *ptr)
{
    igu_debug_print_indent(depth);

    if (ptr)
        printf("%s: " IGU_TERM_PTR "%p" IGU_TERM_NONE "\n", label, ptr);
    else
        printf("%s: " IGU_TERM_PTR "0x0" IGU_TERM_NONE "\n", label);
}

void igu_debug_print_string(size_t depth, const char *label, const char *str)
{
    igu_debug_print_indent(depth);

    if (str)
        printf("%s: " IGU_TERM_STR "\"%s\"  " IGU_TERM_NONE "\n", label, str);
    else
        printf("%s: " IGU_TERM_STR "\"\"" IGU_TERM_NONE "\n", label);
}

#define IGU_COLOR_ARGB(x)       \
    (IGU_COLOR_A(x) << 24 |     \
     IGU_COLOR_R(x) << 16 |     \
     IGU_COLOR_G(x) << 8  |     \
     IGU_COLOR_B(x))

void igu_debug_print_color(size_t depth, const char *label, uint32_t color)
{
    igu_debug_print_indent(depth);

    printf("%s: " IGU_TERM_COLOR "#%08x %s" IGU_TERM_NONE "\n", label,
           IGU_COLOR_ARGB(color), igu_color_to_string(color) ? : "");
}

#define IGU_VALUE_FMT "%.1f%s %.1f%s %.1f%s %.1f%s"

#define IGU_VALUE_TYPE_HINT(value) \
    ((value).type == IGU_VALUE_ABSOLUTE ? "px" : "%")

void igu_debug_print_value4(size_t depth, const char *label,
                            const igu_value4_t *value)
{
    igu_debug_print_indent(depth);

    printf("%s: " IGU_TERM_VALUE IGU_VALUE_FMT IGU_TERM_NONE "\n", label,
           IGU_X(*value).value, IGU_VALUE_TYPE_HINT(IGU_X(*value)),
           IGU_Y(*value).value, IGU_VALUE_TYPE_HINT(IGU_Y(*value)),
           IGU_Z(*value).value, IGU_VALUE_TYPE_HINT(IGU_Z(*value)),
           IGU_W(*value).value, IGU_VALUE_TYPE_HINT(IGU_W(*value)));
}

void igu_debug_print_children(size_t depth, const char *label, size_t count)
{
    igu_debug_print_indent(depth);

    printf("%s: " IGU_TERM_COMMENT "%zu" IGU_TERM_NONE "\n", label, count);
}
