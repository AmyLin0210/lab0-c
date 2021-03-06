#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

#define BUFFER_SIZE 300

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));

    if (!q)
        return NULL;

    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    list_ele_t *tmp_e = NULL;

    if (q) {
        while (q->head) {
            if (q->head->value)
                free(q->head->value);
            tmp_e = q->head;
            q->head = tmp_e->next;
            free(tmp_e);
        }
        free(q);
    }
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    if (!q)
        return false;

    list_ele_t *newh = malloc(sizeof(list_ele_t));
    if (!newh)
        return false;

    newh->value = malloc(sizeof(char) * (strlen(s) + 1));

    if (!newh->value) {
        free(newh);
        return false;
    }

    snprintf(newh->value, BUFFER_SIZE, "%s", s);
    newh->next = q->head;
    q->head = newh;
    (q->size)++;

    if (!q->tail)
        q->tail = q->head;

    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (!q)
        return false;

    list_ele_t *newt = malloc(sizeof(list_ele_t));

    if (!newt)
        return false;

    newt->value = malloc(sizeof(char) * (strlen(s) + 1));

    if (!newt->value) {
        free(newt);
        return false;
    }

    if (!q->head)
        q->head = newt;
    else
        q->tail->next = newt;

    snprintf(newt->value, BUFFER_SIZE, "%s", s);
    newt->next = NULL;
    q->tail = newt;
    (q->size)++;

    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || !(q->head))
        return false;

    list_ele_t *tmp = q->head;
    snprintf(sp, bufsize, "%s", tmp->value);
    q->head = q->head->next;
    (q->size)--;

    free(tmp->value);
    free(tmp);

    if (!q->head)
        q->tail = NULL;

    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (!q)
        return 0;
    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q || !q->head)
        return;

    list_ele_t *prev = q->head, *next = q->head->next;
    q->tail = q->head;
    q->head->next = NULL;

    while (next) {
        q->head = next;
        next = next->next;
        q->head->next = prev;
        prev = q->head;
    }
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (!q || !q->head || q->size == 1)
        return;

    mergesort(&(q->head), q->size);

    list_ele_t *tmp = q->head;

    while (tmp->next)
        tmp = tmp->next;

    q->tail = tmp;
}

void mergesort(list_ele_t **list, int list_len)
{
    list_ele_t *tmp = *list;
    list_ele_t *front = *list;
    list_ele_t *back = NULL;

    if (list_len <= 1)
        return;

    for (int i = 0; i < (list_len / 2) - 1; ++i) {
        tmp = tmp->next;
    }

    back = tmp->next;
    tmp->next = NULL;

    mergesort(&front, list_len / 2);
    mergesort(&back, list_len - list_len / 2);

    tmp = front;
    list_ele_t **result = &tmp;
    list_ele_t *head =
        (strcasecmp(back->value, front->value) >= 0) ? front : back;

    while (front && back) {
        while (front && strcasecmp(back->value, front->value) >= 0) {
            result = &((*result)->next);
            front = front->next;
        }
        *result = back;
        if (!front) {
            break;
        }
        while (back && strcasecmp(front->value, back->value) >= 0) {
            result = &((*result)->next);
            back = back->next;
        }
        *result = front;
    }

    *list = head;
}
