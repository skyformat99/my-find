#include "stack.h"
#include "evalexpr.h"
#include "utilities.h"
#include "expressions.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

/*int main(void)
{
    char *input[] = {
      "(", "-name", "slt", "-a", "-name", "b", "-o", "-name", "slt", ")", "-a", "-print"
    };
    char **postfix = malloc(sizeof(char *) * 12);
    to_postfix(input, 12, postfix);

    eval("slt", postfix);
    free(postfix);
    return 0;
}*/s

/**
** \fn void to_postfix(char **input, char **postfix)
** \brief Converts list of string in infix representation to postfix.
**
** \param char **input list of string in infix, int len its size,
** char **postfix output.
** \return None.
*/
void to_postfix(char **input, int len, char **postfix)
{
    char *s;
    struct stack *stack = init();
    size_t j = 0;
    for (int i = 0; i < len; ++i)
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

/**
** \fn int is_operator(char *s)
** \brief returns true if s if a recognize operator
**
** \param char *s the string on wich to make test.
** \return 1 if is operator, 0 otherwise.
*/
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

/**
** \fn int is_priority(char *data, char *input)
** \brief returns true if input operator has priority on data
** \param char *s the string on wich to make test.
** \return 1 if is operator, 0 otherwise.
*/
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

/**
** \fn int compute(char *op, int a, int b)
** \brief returns the result of a op b or op a if unary operator
** \param char *op the operator, int a the first member and b the second
** \return the result of a op b
*/
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

/**
** \fn int eval(char **postfix)
** \brief Computes the result of the operation given in postfix notation
** \param char **postfix, the list of string representing the operation in
**  postfix
** \return 1 if postfix is evaluated to true, 0 otherwise
*/
int eval(char *path, char **postfix)
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
      int a = 0;
      int b = 0;
      char *arg = NULL;
      stack = pop(stack, &s);
      char *func = s;
      if (my_strcmp(func, "0") || my_strcmp(func, "1"))
        a = func[0] - '0';
      else if (!my_strcmp(postfix[i], "!"))
      {
        if (my_strcmp(func, "-print"))
        {
          stack = pop(stack, &s);
          if (my_strcmp(s, "0"))
            b = s[0] - '0';
          else if (my_strcmp(s, "1"))
            b = s[0] - '0';
          else
          {
            arg = s;
            stack = pop(stack, &s);
            func = s;
            b = call_function(func, arg, path);
          }
          a = call_function("-print", my_itoa(b), path);
        }
        else
        {
          arg = func;
          stack = pop(stack, &s);
          func = s;
/*          stack = pop(stack, &s);
          arg = s;*/
          a = call_function(func, arg, path);
          stack = pop(stack, &s);
          if (my_strcmp(s, "0"))
            b = s[0] - '0';
          else if (my_strcmp(s, "1"))
            b = s[0] - '0';
          else
          {
            arg = s;
            stack = pop(stack, &s);
            func = s;
            b = call_function(func, arg, path);
          }
        }
      }
      else
      {
        stack = pop(stack, &s);
        char *arg = s;
        a = call_function(func, arg, path);
      }
      result = compute(postfix[i], a, b);
      stack = push(stack, my_itoa(result));
    }
  }

  pop(stack, &s);
  return s[0] - '0';
}
