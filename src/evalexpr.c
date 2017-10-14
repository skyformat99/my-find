#include "stack.h"
#include "evalexpr.h"
#include "utilities.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

/*int main(void)
{
    char *input[] = {
      "1", "-o", "0", "-a", "0", NULL
    };
    char **postfix = malloc(sizeof(char *) * 6);
    to_postfix(input, postfix);
    printf("eval : %c\n", eval(postfix));
    free(postfix);
    return 0;
}*/

void to_postfix(char **input, char **postfix)
{
    char *s;
    struct stack *stack = init();
    size_t j = 0;
    for (size_t i = 0; input[i] != NULL; ++i)
    {
        if (!is_operator(input[i]) && !my_strcmp(input[i], "(")
            && !my_strcmp(input[i], ")"))
        {
            postfix[j] = input[i];
            j++;
        }

        if (my_strcmp(input[i], "("))
            stack = push(stack, input[i]);
        if (my_strcmp(input[i], ")"))
        {
            for (; stack->data && !my_strcmp(stack->data, "("); ++j)
            {
                stack = pop(stack, &s);
                postfix[j] = s;
            }
            stack = pop(stack, &s);
        }
        if (is_operator(input[i]))
        {
            if (!stack->data || my_strcmp(stack->data, "("))
                stack = push(stack, input[i]);
            else
            {
                for (; stack->data && !my_strcmp(stack->data, "(")
                       && !is_priority(input[i], stack->data); ++j)
                {
                       stack = pop(stack, &s);
                       postfix[j] = s;
                }
                stack = push(stack, input[i]);
           }
        }
    }
    for (; stack->data; ++j)
    {
        stack = pop(stack, &s);
        postfix[j] = s;
    }

    postfix[j] = NULL;
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

int compute(char *op, int a, int b)
{
  if (my_strcmp(op, "-o"))
    return a || b;
  if (my_strcmp(op, "-a"))
    return a && b;
  if (my_strcmp(op, "!"))
    return !a;
  else
    return -1;
}

char *my_itoa(int a)
{
  if (a == 0)
    return "0";
  return "1";
}

int eval(char **postfix)
{
    struct stack *stack = init();
    char *s;
    int result;
    for (size_t i = 0; postfix[i] != NULL; i++)
    {
      if (!is_operator(postfix[i]))
          stack = push(stack, postfix[i]);
      if (is_operator(postfix[i]))
      {
        if (my_strcmp(postfix[i], "!"))
        {
          stack = pop(stack, &s);
          int a = s[0] - '0';
          result = compute(postfix[i], a, 0);
        }
        else
        {
          stack = pop(stack, &s);
          int a = s[0] - '0';
          stack = pop(stack, &s);
          int b = s[0] - '0';
          result = compute(postfix[i], a, b);
        }
        stack = push(stack, my_itoa(result));
      }
    }
    pop(stack, &s);
    //free(stack);
    return s[0];
}
