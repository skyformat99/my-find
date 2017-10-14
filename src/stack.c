#include "stack.h"
#include "utilities.h"
#include <stdlib.h>
#include <string.h>

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
    struct stack *new = malloc(sizeof(struct stack));
    if (!new)
        return NULL;
    char *string = malloc(my_strlen(data)+1);
    strcpy(string, data);
    new->data = string;
    new->next = head;
    return new;
}

/**
** \fn struct stack *pop(struct stack *head, char **data)
** \brief pops the head of the stack
** \return the new stack
*/
struct stack *pop(struct stack *head, char **data)
{
    *data = malloc(my_strlen(head->data) + 1);
    strcpy(*data, head->data);
    struct stack *new = head->next;
    free(head->data);
    free(head);
    return new;
}

/**
** \fn void free_stack(struct stack *stack)
** \brief free the stack
** \return void
*/
void free_stack(struct stack *stack)
{
  char *data;
  while (stack->next)
    stack = pop(stack, &data);
  free(stack->data);
  free(stack);
}
