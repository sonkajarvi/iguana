/**
 * Copyright (c) 2026, sonkajarvi
 *
 * Licensed under the BSD 2-Clause License.
 * The full license can be found in the LICENSE.txt file.
 */

#ifndef __WIDGET_H
#define __WIDGET_H

#include <stddef.h>
#include <stdint.h>

#define WIDGET_RELAYOUT(wgt)        ((wgt)->relayout(wgt))
// #define WIDGET_REPAINT(wgt, wnd)    ((wgt)->repaint((wgt), (wnd)))
#define WIDGET_FREE(wgt)            ((wgt)->free(wgt))
#define WIDGET_PRINT(wgt)           ((wgt)->print(wgt))

#define __container_of(ptr, type, member)         \
    ({                                            \
        void *__ptr = (void *)(ptr);              \
        (type *)(__ptr - offsetof(type, member)); \
    })

/**
 * struct widget - The base widget structure.
 * @parent: Pointer to the parent widget; set by parent.
 * @prev: Pointer to the previous entry; set and used by parent.
 * @next: See above.
 * @name: Name of the widget.
 * @relayout: Called to recalculate the layout of the widget and its children.
 * @free: Called to free all allocated resources owned by the widget.
 * @print: Called to output information about the widget, and optionally, its
 *         children, to the console.
 */
struct widget {
    struct widget *parent;
    struct widget *prev, *next;

    const char *name;
    uint32_t x, y, width, height;

    void (*relayout)(struct widget *);
    // void (*repaint)(struct widget *);
    void (*free)(struct widget *);
    void (*print)(struct widget *);
};

#endif /* !__WIDGET_H */
