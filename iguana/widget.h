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

typedef uint32_t igu_rect_t[4];

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

/*
 * igu_rect_with_margin - Calculate an area, with margin applied, for painting
 * the widget.
 * @wgt: The widget to use.
 * @out: The rect to write to.
 */
static inline void igu_rect_with_margin(struct igu_widget *wgt, igu_rect_t out)
{
    uint32_t left, top;

    left = igu_value(IGU_LEFT(wgt->margin), IGU_WIDTH(wgt->rect));
    top = igu_value(IGU_TOP(wgt->margin), IGU_HEIGHT(wgt->rect));

    IGU_X(out) = IGU_X(wgt->rect) + left;
    IGU_Y(out) = IGU_Y(wgt->rect) + top;
    IGU_WIDTH(out) = IGU_WIDTH(wgt->rect) - left -
                     igu_value(IGU_RIGHT(wgt->margin), IGU_WIDTH(wgt->rect));
    IGU_HEIGHT(out) = IGU_HEIGHT(wgt->rect) - top -
                      igu_value(IGU_BOTTOM(wgt->margin),
                                IGU_HEIGHT(wgt->rect));
}

/**
 * igu_rect_with_margin_padding - Calculate an area, with margin and padding
 * applied, for calculating the areas for the widget's children.
 * @wgt: The widget to use.
 * @out: The rect to write to.
 */
static inline void igu_rect_with_margin_padding(struct igu_widget *wgt,
                                                igu_rect_t out)
{
    uint32_t left, top;

    igu_rect_with_margin(wgt, out);

    left = igu_value(IGU_LEFT(wgt->padding), IGU_WIDTH(wgt->rect));
    top = igu_value(IGU_TOP(wgt->padding), IGU_HEIGHT(wgt->rect));

    IGU_X(out) += left;
    IGU_Y(out) += top;
    IGU_WIDTH(out) -= left + igu_value(IGU_RIGHT(wgt->padding),
                                       IGU_WIDTH(wgt->rect));
    IGU_HEIGHT(out) -= top + igu_value(IGU_TOP(wgt->padding),
                                       IGU_HEIGHT(wgt->rect));
}

#endif /* !_IGUANA_WIDGET_H */
