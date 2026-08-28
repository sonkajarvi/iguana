/**
 * Copyright (c) 2026, sonkajarvi
 *
 * Licensed under the BSD 2-Clause License.
 * The full license can be found in the LICENSE.txt file.
 */

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iguana/event.h>
#include <iguana/window.h>

#define IGU_EVENT_QUEUE_INITIAL_COUNT 64

int igu_event_queue_init(struct igu_event_queue *queue)
{
    struct igu_event *buf;

    if (!queue)
        return -EINVAL;

    queue->len = IGU_EVENT_QUEUE_INITIAL_COUNT;
    buf = malloc(queue->len * sizeof(*buf));
    if (!buf)
        return -ENOMEM;

    queue->buf = buf;
    queue->pop_index = 0;
    queue->push_index = 0;

    printf(INFO "EVENT QUEUE: initialized: len: %zu\n", queue->len);

    return 0;
}

void igu_event_queue_free(struct igu_event_queue *queue)
{
    if (!queue || !queue->buf)
        return;

    free(queue->buf);

    printf(INFO "EVENT QUEUE: freed\n");
}

int igu_event_queue_push(struct igu_event_queue *queue,
                         struct igu_event *event)
{
    struct igu_event *buf;

    if (!queue || !event)
        return -EINVAL;

    if (igu_event_queue_is_empty(queue)) {
        /*
         * To save space, reset the indices if the buffer is empty.
         */
        queue->pop_index = 0;
        queue->push_index = 0;

    } else if (queue->push_index == queue->len - 1) {
        /*
         * Double the buffer capacity if the buffer is full.
         */
        queue->len *= 2;
        buf = realloc(queue->buf, queue->len * sizeof(*buf));
        if (!buf)
            return -ENOMEM;

        queue->buf = buf;
    }

    memcpy(&queue->buf[queue->push_index], event, sizeof(*event));
    queue->push_index++;

    return 0;
}

int igu_event_queue_pop(struct igu_event_queue *queue,
                        struct igu_event *event)
{
    if (!queue || !event)
        return -EINVAL;

    if (igu_event_queue_is_empty(queue))
        return -ENOENT;

    memcpy(event, &queue->buf[queue->pop_index], sizeof(*event));
    queue->pop_index++;

    return 0;
}

int igu_event_queue_push_mouse_event(struct igu_event_queue *queue, int button,
                                     bool down)
{
    struct igu_event event;

    if (!queue || button < IGU_MOUSE_LEFT || button > IGU_MOUSE_X3)
        return -EINVAL;

    event.type = IGU_EVENT_TYPE_MOUSE;
    event.mouse.button = button;
    event.mouse.down = down;

    return igu_event_queue_push(queue, &event);
}

int igu_event_queue_push_mouse_move_event(struct igu_event_queue *queue, int x,
                                          int y)
{
    struct igu_event event;

    if (!queue || x < 0 || y < 0)
        return -EINVAL;

    event.type = IGU_EVENT_TYPE_MOUSE_MOVE;
    event.mouse_move.x = x;
    event.mouse_move.y = y;

    return igu_event_queue_push(queue, &event);
}

int igu_event_queue_push_mouse_scroll_event(struct igu_event_queue *queue,
                                            int value)
{
    struct igu_event event;

    if (!queue)
        return -EINVAL;

    event.type = IGU_EVENT_TYPE_MOUSE_SCROLL;
    event.mouse_scroll.value = value;

    return igu_event_queue_push(queue, &event);
}

int igu_event_queue_push_window_move_event(struct igu_event_queue *queue,
                                           int x, int y)
{
    struct igu_event event;

    if (!queue || x < 0 || x < 0)
        return -EINVAL;

    event.type = IGU_EVENT_TYPE_WINDOW_MOVE;
    event.window_move.x = x;
    event.window_move.y = y;

    return igu_event_queue_push(queue, &event);
}

int igu_event_queue_push_window_resize_event(struct igu_event_queue *queue,
                                             int width, int height)
{
    struct igu_event event;

    if (!queue || width < 0 || width < 0)
        return -EINVAL;

    event.type = IGU_EVENT_TYPE_WINDOW_RESIZE;
    event.window_resize.width = width;
    event.window_resize.height = height;

    return igu_event_queue_push(queue, &event);
}
