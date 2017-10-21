/**
** \file stack.c
** \brief Functions related to the stack structure.
*/

#include "stack.h"
#include "utilities.h"
#include <stdlib.h>

/**
** \fn struct stack *init(void)
** \brief init a new stack
** \return the new stack
*/
struct stack *init(void)
{
    struct stack *new = malloc(sizeof(struct stack));
    if (!new)
            return NULL;
    new->next = NULL;
    new->data = NULL;
    return new;
}
/**
** \fn struct stack *push(struct stack *head, char *data)
** \brief push a string into the stack
** \return the new stack
*/
struct stack *push(struct stack *head, char *data)
{
    struct stack *new = init();
    new->data = data;
    new->next = head;
    return new;
}
/**
** \fn struct stack *pop(struct stack *head, char **data)
** \brief pops the head of the stack
** \return the new stack
*/
char *pop(struct stack **head)
{
    char *s = (*head)->data;
    struct stack *tmp = (*head)->next;
    free(*head);
    *head = tmp;
    return s;
}
/**
** \fn void free_stack(struct stack *stack)
** \brief free the stack
** \return void
*/
void free_stack(struct stack *stack)
{
  while (stack->next)
  {
    struct stack *tmp = stack->next;
    free(stack);
    stack = tmp;
  }

  free(stack->data);
  free(stack);
}
