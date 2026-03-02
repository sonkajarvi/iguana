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

#define WIDGET_RELAYOUT(wgt)        ((wgt)->vtable->relayout(wgt))
#define WIDGET_REPAINT(wgt)         ((wgt)->vtable->repaint(wgt))
#define WIDGET_FREE(wgt)            ((wgt)->vtable->free(wgt))
#define WIDGET_PRINT(wgt)           ((wgt)->vtable->print(wgt))

#define __container_of(ptr, type, member)         \
    ({                                            \
        void *__ptr = (void *)(ptr);              \
        (type *)(__ptr - offsetof(type, member)); \
    })

/**
 * struct widget - The base widget structure.
 * @vtable: Pointer to the widget's virtual function table.
 * @parent: Pointer to the parent widget; set by parent.
 * @prev:   Pointer to the previous entry; set and used by parent.
 * @next:   See above.
 * @name:   Name of the widget; may be null.
 * @x:      Position of the widget, on the X axis.
 * @y:      Position of the widget, on the Y axis.
 * @width:  The width of the widget.
 * @height: The height of the widget.
 */
struct widget {
    const struct widget_vtable *vtable;

    struct window *window;
    struct widget *parent;
    struct widget *prev, *next;

    const char *name;
    uint32_t x, y, width, height;
};

/**
 * struct widget_vtable - Structure for the widget virtual function table.
 * @relayout: Called to recursively recalculate the layout and the available
 *            area of the widget and its children.
 * @repaint:  Called to recursively redraw the widget and its children.
 * @free:     Called to free all resources owned by the widget.
 * @print:    Called to output information about the widget, and its children,
 *            to the console.
 */
struct widget_vtable {
    void (*relayout)(struct widget *wgt);
    void (*repaint)(struct widget *wgt);
    void (*free)(struct widget *wgt);
    void (*print)(struct widget *wgt);
};

#endif /* !__WIDGET_H */
