#ifndef STACK_H
#define STACK_H

/**
 ** \struct stack
 ** \brief A minimalist implementation of a stack of string structure
 */
struct stack
{
    char *data;
    struct stack *next;
};

struct stack *init(void);
struct stack *push(struct stack *head, char *data);
char *pop(struct stack **head);
void free_stack(struct stack *stack);

#endif
