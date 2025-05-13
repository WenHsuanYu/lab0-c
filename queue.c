#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

#ifndef strlcpy
#define strlcpy(dst, sr, sz) snprintf(dst, sz, "%s", sr)
#endif

/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *head = malloc(sizeof(struct list_head));
    if (!head)
        return NULL;

    INIT_LIST_HEAD(head);
    return head;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (!head)
        return;

    struct list_head *pos, *safe;

    list_for_each_safe(pos, safe, head) {
        element_t *item = list_entry(pos, element_t, list);
        q_release_element(item);
    }

    free(head);
}

/* Create a new element */
static element_t *q_new_element(char *s)
{
    element_t *item = malloc(sizeof(element_t));
    if (!item)
        return NULL;

    item->value = strdup(s);
    if (!item->value) {
        free(item);
        return NULL;
    }
    INIT_LIST_HEAD(&item->list);
    return item;
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head)
        return false;

    element_t *item = q_new_element(s);
    if (!item)
        return false;

    list_add(&item->list, head);

    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    return q_insert_head(head->prev, s);
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;

    element_t *item = list_first_entry(head, element_t, list);
    list_del(&item->list);

    if (sp && bufsize)
        strlcpy(sp, item->value, bufsize);

    return item;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    return q_remove_head(head->prev->prev, sp, bufsize);
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;

    int len = 0;
    struct list_head *pos;

    list_for_each(pos, head) {
        len++;
    }
    return len;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (!head || list_empty(head))
        return false;

    struct list_head *left = head->next;
    struct list_head *right = head->prev;
    while (left != right && left->next != right) {
        left = left->next;
        right = right->prev;
    }
    list_del(right);
    element_t *item = list_entry(right, element_t, list);
    q_release_element(item);
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    if (!head)
        return false;

    element_t *curr, *safe;
    /* the last repeated node */
    bool has_dup = false;

    list_for_each_entry_safe(curr, safe, head, list) {
        // check if curr->value is equal to the next node value
        bool is_equal =
            &safe->list != head && !strcmp(curr->value, safe->value);
        if (is_equal || has_dup) {
            has_dup = true;
            list_del(&curr->list);
            q_release_element(curr);
            has_dup = is_equal && has_dup;
        }
    }

    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    q_reverseK(head, 2);
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head)
        return;

    struct list_head *it;
    struct list_head *safe;
    list_for_each_safe(it, safe, head)
        list_move(it, head);
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
    if (!head || list_empty(head))
        return;

    struct list_head *it, *safe, *begin;
    int count = k;
    begin = head;
    list_for_each_safe(it, safe, head) {
        if (--count)
            continue;
        LIST_HEAD(tmp);
        count = k;
        list_cut_position(&tmp, begin, it);
        q_reverse(&tmp);
        list_splice_init(&tmp, begin);
        /* The iterator will be changed */
        begin = safe->prev;
    }
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend) {}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
