﻿/**
 * File:   event_queue.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  event_queue
 *
 * Copyright (c) 2018 - 2018  Li XianJing <xianjimli@hotmail.com>
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2018-03-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
#include "base/event_queue.h"

event_queue_t* event_queue_create(uint16_t capacity) {
  uint16_t size = 0;
  event_queue_t* q = NULL;
  return_value_if_fail(capacity > 1, NULL);

  size = sizeof(event_queue_t) + (capacity - 1) * sizeof(event_all_t);
  q = (event_queue_t*)TKMEM_ALLOC(size);
  return_value_if_fail(q != NULL, NULL);

  memset(q, 0x00, size);
  q->capacity = capacity;

  return q;
}

ret_t event_queue_recv(event_queue_t* q, event_all_t* e) {
  return_value_if_fail(q != NULL && e != NULL, RET_BAD_PARAMS);
  if (q->r != q->w || q->full) {
    memcpy(e, q->events + q->r, sizeof(*e));
    if ((q->r + 1) < q->capacity) {
      q->r++;
    } else {
      q->r = 0;
    }
    q->full = FALSE;

    return RET_OK;
  }

  return RET_FAIL;
}

ret_t event_queue_send(event_queue_t* q, const event_all_t* e) {
  return_value_if_fail(q != NULL && e != NULL, RET_BAD_PARAMS);
  if (q->r != q->w || !q->full) {
    memcpy(q->events + q->w, e, sizeof(*e));
    if ((q->w + 1) < q->capacity) {
      q->w++;
    } else {
      q->w = 0;
    }
    if (q->r == q->w) {
      q->full = TRUE;
    }

    return RET_OK;
  }

  return RET_FAIL;
}

ret_t event_queue_replace_last(event_queue_t* q, const event_all_t* e) {
  return_value_if_fail(q != NULL && e != NULL, RET_BAD_PARAMS);
  if (q->w > 0) {
    memcpy(q->events + q->w - 1, e, sizeof(*e));
  } else {
    memcpy(q->events + q->capacity - 1, e, sizeof(*e));
  }

  return RET_OK;
}

ret_t event_queue_destroy(event_queue_t* q) {
  return_value_if_fail(q != NULL, RET_BAD_PARAMS);
  TKMEM_FREE(q);

  return RET_OK;
}
