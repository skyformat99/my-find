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
struct stack *pop(struct stack *head, char **data);
void free_stack(struct stack *stack);

#endif
