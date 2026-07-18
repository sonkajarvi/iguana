/**
 * Copyright (c) 2026, sonkajarvi
 *
 * Licensed under the BSD 2-Clause License.
 * The full license can be found in the LICENSE.txt file.
 */

#include <iguana/rect.h>
#include <iguana/widget.h>

void igu_widget_rect_with_margin(struct igu_widget *widget, igu_rect_t out)
{
    float left, top;

    left = igu_value(IGU_LEFT(widget->margin), IGU_WIDTH(widget->rect));
    top = igu_value(IGU_TOP(widget->margin), IGU_HEIGHT(widget->rect));

    IGU_X(out) = IGU_X(widget->rect) + left;
    IGU_Y(out) = IGU_Y(widget->rect) + top;
    IGU_WIDTH(out) = IGU_WIDTH(widget->rect) - left -
                     igu_value(IGU_RIGHT(widget->margin), IGU_WIDTH(widget->rect));
    IGU_HEIGHT(out) = IGU_HEIGHT(widget->rect) - top -
                      igu_value(IGU_BOTTOM(widget->margin),
                                IGU_HEIGHT(widget->rect));
}

void igu_widget_rect_with_margin_and_padding(struct igu_widget *widget,
                                             igu_rect_t out)
{
    float left, top;

    igu_widget_rect_with_margin(widget, out);

    left = igu_value(IGU_LEFT(widget->padding), IGU_WIDTH(widget->rect));
    top = igu_value(IGU_TOP(widget->padding), IGU_HEIGHT(widget->rect));

    IGU_X(out) += left;
    IGU_Y(out) += top;
    IGU_WIDTH(out) -= left + igu_value(IGU_RIGHT(widget->padding),
                                       IGU_WIDTH(widget->rect));
    IGU_HEIGHT(out) -= top + igu_value(IGU_TOP(widget->padding),
                                       IGU_HEIGHT(widget->rect));
}
