#include "queue.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (q != NULL) {
        q->head = NULL;
        q->tail = NULL;
        q->size = 0;
    }
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (q == NULL)
        return;
    while (q->head != NULL) {
        list_ele_t *old_head = q->head;
        q->head = q->head->next;
        free(old_head->value);
        free(old_head);
    }
    q->tail = NULL;
    q->head = NULL;
    /* Free queue structure */
    free(q);
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
    list_ele_t *newh;
    if (q == NULL || s == NULL)
        return false;
    newh = malloc(sizeof(list_ele_t));
    if (newh == NULL)
        return false;
    newh->value = (char *) malloc(strlen(s) + 1);
    if (newh->value == NULL) {
        free(newh);
        return false;
    }
    strncpy(newh->value, s, strlen(s) + 1);
    /* Don't forget to allocate space for the string and copy it */
    /* What if either call to malloc returns NULL? */
    newh->next = q->head;
    q->head = newh;
    if (q->size == 0)
        q->tail = newh;
    q->size++;
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
    list_ele_t *newh;
    if (q == NULL || s == NULL)
        return false;
    newh = malloc(sizeof(list_ele_t));
    if (newh == NULL)
        return false;
    newh->value = (char *) malloc(strlen(s) + 1);
    if (newh->value == NULL) {
        free(newh);
        return false;
    }
    strncpy(newh->value, s, strlen(s) + 1);
    /* To tail */
    newh->next = NULL;
    if (q->size == 0) {
        q->tail = newh;
        q->head = newh;
    } else {
        q->tail->next = newh;
        q->tail = newh;
    }
    q->size++;
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
    if (q == NULL || q->head == NULL)
        return false;
    list_ele_t *old_head = q->head;
    q->head = q->head->next;
    q->size--;

    if (sp != NULL) {
        strncpy(sp, old_head->value, bufsize - 1);
        *(sp + bufsize - 1) = 0;
    }
    free(old_head->value);
    free(old_head);
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (q == NULL)
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
    if (q == NULL || q->head == NULL)
        return;
    list_ele_t *prev = NULL;
    list_ele_t *next = NULL;
    q->tail = q->head;
    while (q->head->next != NULL) {
        next = q->head->next;
        q->head->next = prev;
        prev = q->head;
        q->head = next;
    }
    q->head->next = prev;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
list_ele_t *list_merge(list_ele_t *left, list_ele_t *right)
{
    if (NULL == left)
        return right;
    if (NULL == right)
        return left;
    if (strcmp(left->value, right->value) < 0) {
        left->next = list_merge(left->next, right);
        return left;
    } else {
        right->next = list_merge(left, right->next);
        return right;
    }
}

list_ele_t *list_mergesort(list_ele_t *head)
{
    if (head == NULL || head->next == NULL)
        return head;
    list_ele_t *fast = head->next;
    list_ele_t *slow = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    fast = slow->next;
    slow->next = NULL;
    list_ele_t *l = list_mergesort(head);
    list_ele_t *r = list_mergesort(fast);

    return list_merge(l, r);
}

void q_sort(queue_t *q)
{
    if (q == NULL || q->head == NULL)
        return;
    q->head = list_mergesort(q->head);
    list_ele_t *tmp = q->head;
    while (tmp->next != NULL)
        tmp = tmp->next;
    q->tail = tmp;
}
