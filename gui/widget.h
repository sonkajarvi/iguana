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

#define X(x)            ((x)[0])
#define Y(x)            ((x)[1])
#define WIDTH(x)        ((x)[2])
#define HEIGHT(x)       ((x)[3])

#define TOP(x)          ((x)[0])
#define RIGHT(x)        ((x)[1])
#define BOTTOM(x)       ((x)[2])
#define LEFT(x)         ((x)[3])

#define __container_of(ptr, type, member)         \
    ({                                            \
        void *__ptr = (void *)(ptr);              \
        (type *)(__ptr - offsetof(type, member)); \
    })

typedef uint32_t rect_t[4];

/**
 * struct widget - The base widget structure.
 * @vtable:  Pointer to the widget's virtual function table.
 * @parent:  Pointer to the parent widget; set by parent.
 * @prev:    Pointer to the previous entry; set and used by parent.
 * @next:    See above.
 * @name:    Name of the widget; may be null.
 * @rect:    Available area for the widget; set by parent during re-layout.
 * @margin:  The margin around the widget; affects layout and painting.
 * @padding: The padding around the widget; affects layout.
 */
struct widget {
    const struct widget_vtable *vtable;

    struct window *window;
    struct widget *parent;
    struct widget *prev, *next;

    const char *name;
    rect_t rect;
    rect_t margin, padding;
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

/**
 * rect_with_margin_padding - Calculate an area, with margin and padding applied,
 *      for calculating the areas for the widget's children.
 * @wgt: The widget to use.
 * @out: The rect to write to.
 */
static inline void rect_with_margin_padding(struct widget *wgt, rect_t out)
{
    X(out) = X(wgt->rect) + LEFT(wgt->margin) + LEFT(wgt->padding);
    Y(out) = Y(wgt->rect) + TOP(wgt->margin) + TOP(wgt->padding);

    WIDTH(out) = WIDTH(wgt->rect) - LEFT(wgt->margin)
        - RIGHT(wgt->margin) - LEFT(wgt->padding) - RIGHT(wgt->padding);

    HEIGHT(out) = HEIGHT(wgt->rect) - TOP(wgt->margin)
        - BOTTOM(wgt->margin) - TOP(wgt->padding) - BOTTOM(wgt->padding);
}

/*
 * rect_with_margin - Calculate an area, with margin applied, for painting the widget.
 * @wgt: The widget to use.
 * @out: The rect to write to.
 */
static inline void rect_with_margin(struct widget *wgt, rect_t out)
{
    X(out) = X(wgt->rect) + LEFT(wgt->margin);
    Y(out) = Y(wgt->rect) + TOP(wgt->margin);
    WIDTH(out) = WIDTH(wgt->rect) - LEFT(wgt->margin) - RIGHT(wgt->margin);
    HEIGHT(out) = HEIGHT(wgt->rect) - TOP(wgt->margin) - BOTTOM(wgt->margin);
}

#endif /* !__WIDGET_H */
