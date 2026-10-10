#ifndef AXIN_CLIB_LIST_H
#define AXIN_CLIB_LIST_H

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct axin_list_node {
    struct axin_list_node* prev;
    struct axin_list_node* next;
    void* data; // Borrowed pointer; not owned by the list.
} axin_list_node_t;

typedef struct {
    axin_list_node_t head;
    size_t size;
} axin_list_t;

static inline void axin_list_init(
    axin_list_t* list
) {
    list->head.prev = &list->head;
    list->head.next = &list->head;
    list->head.data = NULL;
    list->size = 0;
}

static inline size_t axin_list_size(
    const axin_list_t* list
) {
    return list->size;
}

static inline bool axin_list_empty(
    const axin_list_t* list
) {
    return list->size == 0;
}

static inline axin_list_node_t* axin_list_front(
    const axin_list_t* list
) {
    return list->head.next;
}

static inline axin_list_node_t* axin_list_back(
    const axin_list_t* list
) {
    return list->head.prev;
}

static inline axin_list_node_t* axin_list_insert_before(
    axin_list_t* list,
    axin_list_node_t* pos, // Must belong to 'list'.
    void* data
) {
    axin_list_node_t* node = malloc(sizeof(axin_list_node_t));
    if (node == NULL) return NULL;
    node->data = data;
    node->prev = pos->prev;
    node->next = pos;
    pos->prev->next = node;
    pos->prev = node;
    list->size++;
    return node;
}

static inline axin_list_node_t* axin_list_insert_after(
    axin_list_t* list,
    axin_list_node_t* pos, // Must belong to 'list'.
    void* data
) {
    axin_list_node_t* node = malloc(sizeof(axin_list_node_t));
    if (node == NULL) return NULL;
    node->data = data;
    node->prev = pos;
    node->next = pos->next;
    pos->next->prev = node;
    pos->next = node;
    list->size++;
    return node;
}

static inline axin_list_node_t* axin_list_push_front(
    axin_list_t* list,
    void* data
) {
    return axin_list_insert_after(list, &list->head, data);
}

static inline axin_list_node_t* axin_list_push_back(
    axin_list_t* list,
    void* data
) {
    return axin_list_insert_before(list, &list->head, data);
}

static inline axin_list_node_t* axin_list_remove(
    axin_list_t* list,
    axin_list_node_t* pos // Must belong to 'list'.
) {
    if (pos == &list->head) return pos;
    pos->prev->next = pos->next;
    pos->next->prev = pos->prev;
    axin_list_node_t* r = pos->next;
    free(pos);
    list->size--;
    return r;
}

static inline void* axin_list_pop_front(
    axin_list_t* list
) {
    if (axin_list_empty(list)) return NULL;
    void* r = list->head.next->data;
    axin_list_remove(list, list->head.next);
    return r;
}

static inline void* axin_list_pop_back(
    axin_list_t* list
) {
    if (axin_list_empty(list)) return NULL;
    void* r = list->head.prev->data;
    axin_list_remove(list, list->head.prev);
    return r;
}

static inline void axin_list_clear(
    axin_list_t* list
) {
    while (!axin_list_empty(list)) {
        axin_list_pop_front(list);
    }
}

// Do not delete the current node during iteration.
#define axin_list_foreach(curr, list) \
    for ((curr) = axin_list_front(list); \
        (curr) != &(list)->head; \
        (curr) = (curr)->next)

// Do not delete the current node during iteration.
#define axin_list_foreach_reverse(curr, list) \
    for ((curr) = axin_list_back(list); \
        (curr) != &(list)->head; \
        (curr) = (curr)->prev)

// The current node may be removed during iteration.
#define axin_list_foreach_safe(curr, tmp, list) \
    for ((curr) = axin_list_front(list), (tmp) = (curr)->next; \
        (curr) != &(list)->head; \
        (curr) = (tmp), (tmp) = (curr)->next)

// The current node may be removed during iteration.
#define axin_list_foreach_reverse_safe(curr, tmp, list) \
    for ((curr) = axin_list_back(list), (tmp) = (curr)->prev; \
        (curr) != &(list)->head; \
        (curr) = (tmp), (tmp) = (curr)->prev)

#endif // AXIN_CLIB_LIST_H
