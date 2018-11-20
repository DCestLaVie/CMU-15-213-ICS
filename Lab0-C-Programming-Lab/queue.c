/* 
 * Code for basic C skills diagnostic.
 * Developed for courses 15-213/18-213/15-513 by R. E. Bryant, 2017
 * Modified to store strings, 2018
 */

/*
 * This program implements a queue supporting both FIFO and LIFO
 * operations.
 *
 * It uses a singly-linked list to represent the set of queue elements
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
  Create empty queue.
  Return NULL if could not allocate space.
*/
queue_t *q_new()
{
    queue_t *q =  malloc(sizeof(queue_t));
    if(!q)
      return NULL;
    q->size = 0;
    q->head = NULL;
    q->tail = NULL;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q)
      return;
    list_ele_t *node = q->head;
    list_ele_t *next = NULL;
    while (node) {
      free(node->value);
      next = node->next;
      free(node);
      node = next;
    }
    free(q);
}

/*
  Attempt to insert element at head of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
  Argument s points to the string to be stored.
  The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    if(!q)
      return false;
    list_ele_t *newh = malloc(sizeof(list_ele_t));
    if (!newh)
      return false;
    newh->value = malloc(strlen(s) + 1);
    if(!newh->value) {
      free(newh);
      return false;
    }
    strcpy(newh->value, s);
    newh->next = q->head;
    q->head = newh;
    if(q->size++ == 0)
      q->tail = newh;
    return true;
}


/*
  Attempt to insert element at tail of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
  Argument s points to the string to be stored.
  The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if(!q)
      return NULL;
    if(q->size == 0)
      return q_insert_head(q, s);
    list_ele_t *node = malloc(sizeof(list_ele_t));
    if(!node)
      return false;
    node->next = NULL;
    node->value = malloc(strlen(s) + 1);
    if(!node->value){
      free(node);
      return false;
    }
    strcpy(node->value, s);
    q->tail->next = node;
    q->tail = node;
    q->size++;
    return true;
}

/*
  Attempt to remove element from head of queue.
  Return true if successful.
  Return false if queue is NULL or empty.
  If sp is non-NULL and an element is removed, copy the removed string to *sp
  (up to a maximum of bufsize-1 characters, plus a null terminator.)
  The space used by the list element and the string should be freed.
*/
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if(!q || q->size == 0)
      return false;
    list_ele_t *node = q->head;
    q->head = node->next;
    if(q->size-- == 1)
      q->tail = NULL;
    if(sp && node->value){
      strncpy(sp, node->value, bufsize-1);
      sp[bufsize-1] = '\0';
    }
    free(node->value);
    free(node);
    return true;
}

/*
  Return number of elements in queue.
  Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if(!q)
      return 0;
    return q->size;
}

/*
  Reverse elements in queue
  No effect if q is NULL or empty
  This function should not allocate or free any list elements
  (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
  It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if(!q || q->size <= 1)
      return;
    list_ele_t *node = q->head;
    list_ele_t *next = NULL;
    q->tail = q->head;
    q->head = NULL;
    while(node) {
      next = node->next;
      node->next = q->head;
      q->head = node;
      node = next;
    }
}

