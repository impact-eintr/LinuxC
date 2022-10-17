#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../headers/algorithm.h"

int linkedlist_internal_add(linkedlist_internal_t *list,
                            linkedlist_node_interface *i_node, uint64_t value) {
  if (list == NULL) {
    return 0;
  }
  assert(list->update_head != NULL);
  linkedlist_validate_interface(i_node, ILIST_CONSTRUCT | ILIST_PREV | ILIST_NEXT | ILIST_VALUE);

  uint64_t node = i_node->construct_node();
  i_node->set_node_value(node, value);
  return linkedlist_internal_insert(list, i_node, node);
}

int linkedlist_internal_insert(linkedlist_internal_t *list,
                               linkedlist_node_interface *i_node,
                               uint64_t node) {
  if (list == NULL) {
    return 0;
  }

  assert(list->update_head != NULL);
  linkedlist_validate_interface(i_node, ILIST_PREV | ILIST_NEXT);
  if (list->count == 0) {
    // create a new head
    list->update_head(list, node);
    list->count = 1;

    i_node->set_node_prev(node, node);
    i_node->set_node_next(node, node);
  } else {
    // inset to head
    uint64_t head = list->head;
    uint64_t head_prev = i_node->get_node_prev(head);
    i_node->set_node_next(node, head);
    i_node->set_node_prev(head, node);
    i_node->set_node_prev(node, head_prev);
    i_node->set_node_next(head_prev, node);

    // update head info
    list->update_head(list, node);
    list->count++;
  }
  return 1;
}

int linkedlist_internal_insert_after(linkedlist_internal_t *list,
                                     linkedlist_node_interface *i_node,
                                     uint64_t prev, uint64_t node) {
  if (list == NULL) {
    return 0;
  }
  assert(list->update_head != NULL);
  assert(list->count >= 1);
  assert(prev != (uint64_t)NULL);
  linkedlist_validate_interface(i_node, ILIST_PREV | ILIST_NEXT);

  uint64_t prev_next = i_node->get_node_next(prev);
  i_node->set_node_next(node, prev_next);
  i_node->set_node_prev(prev_next, node);
  i_node->set_node_next(prev, node);
  i_node->set_node_prev(node, prev);
  list->count++;
  return 1;
}

int linkedlist_internal_insert_before(linkedlist_internal_t *list,
                                     linkedlist_node_interface *i_node,
                                     uint64_t next, uint64_t node) {
  if (list == NULL) {
    return 0;
  }
  assert(list->update_head != NULL);
  assert(list->count >= 1);
  assert(next != (uint64_t)NULL);
  linkedlist_validate_interface(i_node, ILIST_PREV | ILIST_NEXT);

  uint64_t next_prev = i_node->get_node_prev(next);
  i_node->set_node_next(node, next);
  i_node->set_node_prev(node, next_prev);
  i_node->set_node_next(next_prev, node);
  i_node->set_node_prev(next, node);
  list->count++;

  if (next == list->head) {
    list->update_head(list, node);
  }
  return 1;
}

int linkedlist_internal_delete(linkedlist_internal_t *list,
                               linkedlist_node_interface *i_node,
                               uint64_t node) {
  if (list == NULL || node == NULL_ID) {
    return 0;
  }
  assert(list->update_head != NULL);
  linkedlist_validate_interface(i_node, ILIST_COMPARE | ILIST_CHECKNULL | ILIST_PREV | ILIST_NEXT);

  // update the prev and next pointers
  // same for the only one node situation
  uint64_t prev = i_node->get_node_prev(node);
  uint64_t next = i_node->get_node_next(node);

  if (i_node->is_null_node(prev) == 0) {
    i_node->set_node_next(prev, next);
  }

  if (i_node->is_null_node(next) == 0) {
    i_node->set_node_prev(next, prev);
  }

  // if this node to be free is the head
  if (i_node->compare_nodes(list->head, node) == 0) {
    list->update_head(list, next);
  }

  // reset the linked list status
  list->count --;

  if (list->count == 0) {
    list->update_head(list, NULL_ID);
  }

  assert(list->count >= 0);
  return 1;
}

uint64_t linkedlist_internal_index(linkedlist_internal_t *list,
                                   linkedlist_node_interface *i_node,
                                   uint64_t index) {

  if (list == NULL || index >= list->count) {
    return NULL_ID;
  }
  linkedlist_validate_interface(i_node, ILIST_NEXT);

  uint64_t p = list->head;
  for (int i = 0;i <= index;++i) {
    p = i_node->get_node_next(p);
  }
  return p;
}

// traverse the linked list
uint64_t linkedlist_internal_next(linkedlist_internal_t *list,
                                  linkedlist_node_interface *i_node) {
  if (list == NULL || i_node->compare_nodes(list->head, NULL_ID) == 0) {
    return NULL_ID;
  }
  assert(list->update_head != NULL);
  linkedlist_validate_interface(i_node, ILIST_NEXT);

  uint64_t old_head = list->head;
  list->update_head(list, i_node->get_node_next(old_head));

  return old_head;
}

void linkedlist_validate_interface(linkedlist_node_interface *i_node,
                                   uint64_t flags) {
  assert(i_node != NULL);

  if ((flags & ILIST_CONSTRUCT) != 0) {
    assert(i_node->construct_node != NULL);
  }

  if ((flags & ILIST_DESTRUCT) != 0) {
    assert(i_node->destruct_node != NULL);
  }

  if ((flags & ILIST_CHECKNULL) != 0) {
    assert(i_node->is_null_node != NULL);
  }

  if ((flags & ILIST_COMPARE) != 0) {
    assert(i_node->compare_nodes != NULL);
  }

  if ((flags & ILIST_PREV) != 0) {
    assert(i_node->get_node_prev != NULL);
    assert(i_node->set_node_prev != NULL);
  }

  if ((flags & ILIST_NEXT) != 0) {
    assert(i_node->get_node_next != NULL);
    assert(i_node->set_node_next != NULL);
  }

  if ((flags & ILIST_VALUE) != 0) {
    assert(i_node->get_node_value != NULL);
    assert(i_node->set_node_value != NULL);
  }
}

/*======================================*/
/*      Default Implementation          */
/*======================================*/
static int is_null_node(uint64_t node_id) {
  if (NULL_ID == node_id) {
    return 1;
  }
  return 0;
}

static uint64_t construct_node() {
  return (uint64_t)malloc(sizeof(linkedlist_node_t));
}

static int destruct_node(uint64_t node_id) {
  if (is_null_node(node_id) == 1) {
    return 0;
  }
  linkedlist_node_t *node = (linkedlist_node_t *)node_id;
  printf("%ld ", node->value);

  free(node);
  return 1;
}


static int compare_nodes(uint64_t first, uint64_t second) {
  return !(first == second); // same return 0
}

static uint64_t get_node_prev(uint64_t node_id) {
  if (is_null_node(node_id) == 1) {
    return NULL_ID;
  }
  return (uint64_t)(((linkedlist_node_t *)node_id)->prev);
}

static int set_node_prev(uint64_t node_id, uint64_t prev_id) {
  if (is_null_node(node_id) == 1) {
    return 0;
  }
  //*(uint64_t *)&(((linkedlist_node_t *)node_id)->prev) = prev_id;
  ((linkedlist_node_t*)node_id)->prev = (linkedlist_node_t*)prev_id;
  return 1;
}

static uint64_t get_node_next(uint64_t node_id) {
  if (is_null_node(node_id) == 1) {
    return NULL_ID;
  }
  return (uint64_t)(((linkedlist_node_t *)node_id)->next);
}

static int set_node_next(uint64_t node_id, uint64_t next_id) {
  if (is_null_node(node_id) == 1) {
    return 0;
  }
  //*(uint64_t *)&(((linkedlist_node_t *)node_id)->next) = next_id;
  ((linkedlist_node_t *)node_id)->next = (linkedlist_node_t*)next_id;
  return 1;
}

static uint64_t get_node_value(uint64_t node_id) {
  if (is_null_node(node_id) == 1) {
    return NULL_ID;
  }
  return (uint64_t)(((linkedlist_node_t *)node_id)->value);
}

static int set_node_value(uint64_t node_id, uint64_t value) {
  if (is_null_node(node_id) == 1) {
    return 0;
  }
  ((linkedlist_node_t *)node_id)->value = value;
  return 1;
}

/*======================================*/
/*          Derive Class                */
/*======================================*/
static linkedlist_node_interface i_node = {.construct_node = &construct_node,
                                           .destruct_node = &destruct_node,
                                           .is_null_node = &is_null_node,
                                           .compare_nodes = &compare_nodes,
                                           .get_node_prev = &get_node_prev,
                                           .set_node_prev = &set_node_prev,
                                           .get_node_next = &get_node_next,
                                           .set_node_next = &set_node_next,
                                           .get_node_value = &get_node_value,
                                           .set_node_value = &set_node_value};

static int update_head(linkedlist_internal_t *this, uint64_t new_head) {
  if (this == NULL) {
    return 0;
  }
  this->head = new_head;
  return 0;
}

// constructor and destructor
linkedlist_t *linkedlist_construct() {
  linkedlist_t *list = malloc(sizeof(linkedlist_t));
  list->base.count = 0;
  list->base.head = 0;
  list->base.update_head = &update_head;
  return list;
}

void linkedlist_free(linkedlist_t *list) {
  if (list == NULL) {
    return;
  }
  linkedlist_internal_t *base = &(list->base);
  assert(base->update_head != NULL);

  int count_copy = base->count;
  for (int i = 0;i < count_copy;++i) {
    uint64_t node = base->head; // get current head
    base->update_head(base, i_node.get_node_next(base->head)); // rearward move head

    if (i_node.compare_nodes(node, base->head) == 0) { // last node
      i_node.destruct_node(node);
    } else {
      uint64_t prev = i_node.get_node_prev(node);
      uint64_t next = i_node.get_node_next(node);

      i_node.set_node_next(prev, next);
      i_node.set_node_prev(next, prev);

      i_node.destruct_node(node);// destroy the old head
    }
  }
  free(list);
}

void linkedlist_add(linkedlist_t *list, uint64_t value) {
  linkedlist_internal_add(&list->base, &i_node, value);
}

void linkedlist_delete(linkedlist_t *list, linkedlist_node_t *node) {
  printf("delete %ld\n", node->value);
  linkedlist_internal_delete(&list->base, &i_node, (uint64_t)node);
  free(node);
}

linkedlist_node_t *linkedlist_next(linkedlist_t *list) {
  return (linkedlist_node_t *)linkedlist_internal_next(&list->base, &i_node);
}

linkedlist_node_t *linkedlist_index(linkedlist_t *list, uint64_t index) {
  return (linkedlist_node_t *)linkedlist_internal_index(&list->base, &i_node, index);
}
