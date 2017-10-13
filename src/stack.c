#include "stack.h"
#include "utilities.h"
#include <stdlib.h>
#include <string.h>

struct stack *init(void)
{
    struct stack *new = malloc(sizeof(struct stack));
    if (!new)
            return NULL;
    new->next = NULL;
    new->data = NULL;
    return new;
}

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

struct stack *pop(struct stack *head, char **data)
{
    *data = malloc(my_strlen(head->data) + 1);
    strcpy(*data, head->data);
    struct stack *new = head->next;
    free(head->data);
    free(head);
    return new;
}

void free_stack(struct stack *stack)
{
  char *data;
  while (stack->next)
    stack = pop(stack, &data);
  free(stack->data);
  free(stack);
}
