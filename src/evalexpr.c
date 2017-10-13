#include "stack.h"
#include "evalexpr.h"
#include "utilities.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

/*int main(void)
{
    char *input[] = {
      "!", "-name", "test", "-o", "-type", "f", "-a", "-type", "d", NULL
    };
    char **postfix = malloc(sizeof(char *) * 10);
    to_postfix(input, postfix);
    for (int i = 0; i < 10; i++)
      printf("%s ", postfix[i]);
    free(postfix);
    return 0;
}
*/
void to_postfix(char **input, char **postfix)
{
    char *s;
    struct stack *stack = init();
    size_t j = 0;
    for (size_t i = 0; input[i] != NULL; i++)
    {
        printf("%zu", i);
        if (!is_operator(input[i]))
        {
            postfix[j] = input[i];
            j++;
        }

        if (my_strcmp(input[i], "("))
            stack = push(stack, input[i]);
        if (my_strcmp(input[i], ")"))
        {
            while (stack->data && !my_strcmp(stack->data, "("))
            {
                stack = pop(stack, &s);
                postfix[j] = s;
                j++;
            }
            stack = pop(stack, &s);
        }
        if (is_operator(input[i]))
        {
            if (!stack->data || my_strcmp(stack->data, "("))
                stack = push(stack, input[i]);
            else
            {
                while (stack->data && my_strcmp(stack->data, "(")
                       && !is_priority(input[i], stack->data))
                {
                       stack = pop(stack, &s);
                       postfix[j] = s;
                       j++;
                }
                stack = push(stack, input[i]);
           }
        }
    }
    while (stack->data)
    {
        stack = pop(stack, &s);
        postfix[j] = s;
        j++;
    }

    free_stack(stack);
}

int is_operator(char *s)
{
    char *operator[] =
    {
        "!", "-o", "-a"
    };

    for (size_t i = 0; i < 3; i++)
        if (my_strcmp(operator[i], s))
            return 1;
    return 0;
}

int is_priority(char *data, char *input)
{
    if (my_strcmp(data, input))
        return 0;
    if (my_strcmp(data, "!"))
        return 1;
    if (my_strcmp(data, "-a") && !my_strcmp(input, "!"))
        return 1;
    return 0;
}
/*
int compute(char op, int a, int b, int *result)
{
    switch (op)
    {
        case '+':
            *result = a + b;
            return 0;
        case '-':
            *result = a - b;
            return 0;
        case '^':
            *result = power(a, b);
            return 0;
        case '*':
            *result = a * b;
            return 0;
        case '/':
            if (b == 0)
                return -1;
            *result = a / b;
            return 0;
        case '%':
            if (b == 0)
                 return -1;
            *result = a % b;
            return 0;
    }

    return 0;
}

static int power(int a, int b)
{
    int result = 1;
    while (a != 0)
    {
        result *= b;
        --a;
    }
    return result;
}

int eval(char *postfix)
{
    struct stack *stack = init();
    char c;
    int result;
    for (size_t i = 0; postfix[i] != '\0'; i++)
    {
        if (postfix[i] >= '0' && postfix[i] <= '9')
            stack = push(stack, postfix[i]);
        if (is_operator(postfix[i]))
        {
            stack = pop(stack, &c);
            int a = c - '0';
            stack = pop(stack, &c);
            int b = c - '0';
            if (compute(postfix[i], a, b, &result) == -1)
                return -42;
            stack = push(stack, '0' + result);
        }
    }
    pop(stack, &c);
    return c;
}
*/
