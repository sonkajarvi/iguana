/**
 * Copyright (c) 2026, sonkajarvi
 *
 * Licensed under the BSD 2-Clause License.
 * The full license can be found in the LICENSE.txt file.
 */

#ifndef _IGUANA_WIDGET_H
#define _IGUANA_WIDGET_H

#include <stddef.h>
#include <stdint.h>

#include <iguana/accessor.h>
#include <iguana/rect.h>
#include <iguana/value.h>

#define WIDGET_RELAYOUT(wgt)        ((wgt)->vtable->relayout(wgt))
#define WIDGET_REPAINT(wgt)         ((wgt)->vtable->repaint(wgt))
#define WIDGET_FREE(wgt)            ((wgt)->vtable->free(wgt))
#define WIDGET_PRINT(wgt)           WIDGET_PRINT_N(wgt, 0)
#define WIDGET_PRINT_N(wgt, depth)  ((wgt)->vtable->print(wgt, depth))

#define igu_container_of(ptr, type, member)       \
    ({                                            \
        void *__ptr = (void *)(ptr);              \
        (type *)(__ptr - offsetof(type, member)); \
    })

/**
 * struct igu_widget - The base widget structure.
 * @vtable:  Pointer to the widget's virtual function table.
 * @parent:  Pointer to the parent widget; set by parent.
 * @prev:    Pointer to the previous entry; set and used by parent.
 * @next:    See above.
 * @name:    Name of the widget; may be null.
 * @rect:    Available area for the widget; set by parent during re-layout.
 * @margin:  The margin around the widget; affects layout and painting.
 * @padding: The padding around the widget; affects layout.
 */
struct igu_widget {
    const struct igu_widget_vtable *vtable;

    struct igu_window *window;
    struct igu_widget *parent;
    struct igu_widget *prev, *next;

    const char *name;
    igu_rect_t rect;
    igu_value4_t margin, padding;
};

/**
 * struct igu_widget_vtable - Structure for the widget virtual function table.
 * @relayout: Called to recursively recalculate the layout and the available
 *            area of the widget and its children.
 * @repaint:  Called to recursively redraw the widget and its children.
 * @free:     Called to free all resources owned by the widget.
 * @print:    Called to output information about the widget, and its children,
 *            to the console.
 */
struct igu_widget_vtable {
    void (*relayout)(struct igu_widget *wgt);
    void (*repaint)(struct igu_widget *wgt);
    void (*free)(struct igu_widget *wgt);
    void (*print)(struct igu_widget *wgt, int depth);
};

/**
 * igu_widget_rect_with_margin() - Get widget area, with margin
 * @widget:         The widget to use
 * @out:            The rect to write to
 *
 * Calculate a widget's area, with margin applied, for painting.
 */
void igu_widget_rect_with_margin(struct igu_widget *widget, igu_rect_t out);

/**
 * igu_widget_rect_with_margin_and_padding() - Get widget area, with margin and
 *                                             padding
 * @widget:         The widget to use
 * @out:            The rect to write to
 *
 * Calculate a widget's area, with margin and padding applied, for calculating
 * the areas for the widget's children.
 */
void igu_widget_rect_with_margin_and_padding(struct igu_widget *widget,
                                             igu_rect_t out);

#endif /* !_IGUANA_WIDGET_H */
