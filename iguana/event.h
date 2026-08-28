/**
 * Copyright (c) 2026, sonkajarvi
 *
 * Licensed under the BSD 2-Clause License.
 * The full license can be found in the LICENSE.txt file.
 */

#ifndef _IGUANA_EVENT_H
#define _IGUANA_EVENT_H

#include <stdbool.h>
#include <stddef.h>

enum {
    IGU_MOUSE_LEFT,
    IGU_MOUSE_MIDDLE,
    IGU_MOUSE_RIGHT,
    IGU_MOUSE_X1,
    IGU_MOUSE_X2,
    IGU_MOUSE_X3,
};

enum {
    IGU_EVENT_TYPE_MOUSE,
    IGU_EVENT_TYPE_MOUSE_MOVE,
    IGU_EVENT_TYPE_MOUSE_SCROLL,
    IGU_EVENT_TYPE_WINDOW_MOVE,
    IGU_EVENT_TYPE_WINDOW_RESIZE,
};

struct igu_event {
    int type;

    union {
        struct {
            int button;
            bool down;
        } mouse;
        struct {
            int x;
            int y;
        } mouse_move;
        struct {
            int value;
        } mouse_scroll;
        struct {
            int x;
            int y;
        } window_move;
        struct {
            int width;
            int height;
        } window_resize;
    };
};

struct igu_event_queue {
    struct igu_event *buf;
    size_t len;
    size_t pop_index, push_index;
};

int igu_event_queue_init(struct igu_event_queue *queue);
void igu_event_queue_free(struct igu_event_queue *queue);

static inline bool igu_event_queue_is_empty(struct igu_event_queue *queue)
{
    return queue ? queue->pop_index == queue->push_index : false;
}

/**
 * igu_event_queue_push - Push an event into the queue
 * @queue:      The queue to push to
 * @event:      The event to push
 *
 * Returns 0 if successful, a negative error code otherwise.
 */
int igu_event_queue_push(struct igu_event_queue *queue,
                         struct igu_event *event);

/**
 * igu_event_queue_pop - Pop an event from the queue
 * @queue:      The queue to pop from
 * @event:      The event to write out to
 *
 * Returns 0 if successful, -ENOENT if nothing to pop, and a negative error
 * code otherwise.
 */
int igu_event_queue_pop(struct igu_event_queue *queue,
                        struct igu_event *event);

/*
 * Here are some convenience functions for pushing events to queues.
 */
int igu_event_queue_push_mouse_event(struct igu_event_queue *queue,
                                     int button, bool down);
int igu_event_queue_push_mouse_move_event(struct igu_event_queue *queue,
                                          int x, int y);
int igu_event_queue_push_mouse_scroll_event(struct igu_event_queue *queue,
                                            int value);

int igu_event_queue_push_window_move_event(struct igu_event_queue *queue,
                                           int x, int y);
int igu_event_queue_push_window_resize_event(struct igu_event_queue *queue,
                                             int width, int height);

#endif /* !_IGUANA_EVENT_H */
