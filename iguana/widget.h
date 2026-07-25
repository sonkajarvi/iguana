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
    struct igu_widget *children;
    size_t num_children;
    struct igu_widget *prev, *next;

    const char *name;
    igu_rect_t rect;
    igu_value4_t margin, padding;

    /* events */
    void (*on_mouse_enter)(struct igu_widget *widget);
    void (*on_mouse_exit)(struct igu_widget *widget);
};

#define igu_widget_invoke_event(_widget, _event, ...)       \
    do {                                                    \
        if ((_widget) && (_widget)->_event)                 \
            (_widget)->_event((_widget), ##__VA_ARGS__);    \
    } while (0)

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
    void (*relayout)(struct igu_widget *widget);
    void (*redraw)(struct igu_widget *widget);
    void (*free)(struct igu_widget *widget);
    void (*print)(struct igu_widget *widget, int depth);
};

static inline void igu_widget_relayout(struct igu_widget *widget)
{
    widget->vtable->relayout(widget);
}

static inline void igu_widget_redraw(struct igu_widget *widget)
{
    widget->vtable->redraw(widget);
}

static inline void igu_widget_free(struct igu_widget *widget)
{
    widget->vtable->free(widget);
}

static inline void __igu_widget_print(struct igu_widget *widget, int depth)
{
    widget->vtable->print(widget, depth);
}

static inline void igu_widget_print(struct igu_widget *widget)
{
    __igu_widget_print(widget, 0);
}

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
