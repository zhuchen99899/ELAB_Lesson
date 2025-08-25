
//
// iterator.c
//
// Copyright (c) 2010 TJ Holowaychuk <tj@vision-media.ca>
//

#include "list.h"

/*
 * Allocate a new list_iterator_t. NULL on failure.
 * Accepts a direction, which may be LIST_HEAD or LIST_TAIL.
 */

list_iterator_t *
list_iterator_new(list_t *list, list_direction_t direction) {
  list_node_t *node = direction == LIST_HEAD
    ? list->head
    : list->tail;
  return list_iterator_new_from_node(node, direction);
}

/*
 * Allocate a new list_iterator_t with the given start
 * node. NULL on failure.
 */

list_iterator_t *
list_iterator_new_from_node(list_node_t *node, list_direction_t direction) {
  list_iterator_t *self=LIST_MALLOC(sizeof(list_iterator_t));
  if (!self)
    return NULL;
  self->next = node;
  self->direction = direction;
  return self;
}

/*
 * Make a list_iterator_t through first arg.
 * Accepts a direction, which may be LIST_HEAD or LIST_TAIL.
 */
int32_t 
list_iterator_make(list_iterator_t *list_iterator, list_t *list, 
    list_direction_t direction)
{
    int32_t ret = -1;
    list_node_t *node;
    if((NULL == list_iterator) || (NULL == list)) goto exit;

    node = direction == LIST_HEAD
    ? list->head
    : list->tail;

    list_iterator_make_from_node(list_iterator, node, direction);
    
    ret = 0;

exit:
    return ret;
}

/*
 * Make a list_iterator_t through first arg, with the given start
 * node. NULL on failure.
 */
int32_t 
list_iterator_make_from_node
(list_iterator_t *list_iterator, list_node_t *node, list_direction_t direction)
{
    int32_t ret = -1;

    if((NULL == list_iterator) || (NULL == node)) goto exit;
    
    list_iterator->next = node;
    list_iterator->direction = direction;
    
    ret = 0;

exit: 
    return ret;
}

/*
 * Return the next list_node_t or NULL when no more
 * nodes remain in the list.
 */

list_node_t *
list_iterator_next(list_iterator_t *self) {
  list_node_t *curr = self->next;
  if (curr) {
    self->next = self->direction == LIST_HEAD
      ? curr->next
      : curr->prev;
  }
  return curr;
}

/*
 * Free the list iterator.
 */

void
list_iterator_destroy(list_iterator_t *self) {
  LIST_FREE(self);
  self = NULL;
}
