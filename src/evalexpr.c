#include "stack.h"
#include "evalexpr.h"
#include "utilities.h"
#include "expressions.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

static int eval_second_operand(char *path, struct stack **stack, int a);
/**
** \fn void to_postfix(char **input, char **postfix)
** \brief Converts list of string in infix representation to postfix.
**
** \param char **input list of string in infix, int len its size,
** char **postfix output.
** \return the len of the expression in postfix.
*/
int to_postfix(char **input, int len, char **postfix)
{
    struct stack *stack = init();
    int j = 0;
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
                postfix[j] = pop(&stack);
            pop(&stack);
        }
        if (is_operator(input[i]))
        {
            if (!stack->data || my_strcmp(stack->data, "("))
                stack = push(stack, input[i]);
            else
            {
                for (; stack->data && !my_strcmp(stack->data, "(")
                       && !is_priority(input[i], stack->data); ++j)
                       postfix[j] = pop(&stack);
                stack = push(stack, input[i]);
            }
        }
    }
    for (; stack->data; ++j)
        postfix[j] = pop(&stack);

    free_stack(stack);
    return j;
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

static int eval_second_operand(char *path, struct stack **st, int a)
{
  char *arg = pop(st);
  if (my_strcmp(arg, "0") || my_strcmp(arg, "1"))
    return arg[0] - '0';
  if (my_strcmp(arg, "-print"))
    return call_function("-print", my_itoa(a), path);
  else
  {
    char *func = pop(st);
    return call_function(func, arg, path);
  }
}

int eval(char *path, char **postfix, int len)
{
  struct stack *stack = init();
  for (int i = 0; i < len; ++i)
  {
    if (!is_operator(postfix[i]))
      stack = push(stack, postfix[i]);
    else
    {
      int a = 0;
      int b = 0;
      char *arg = pop(&stack);
      if (my_strcmp(arg, "0") || my_strcmp(arg, "1"))
      {
        b = arg[0] - '0';
        a = eval_second_operand(path, &stack, 1);
      }
      else if (my_strcmp(arg, "-print"))
      {
        a = eval_second_operand(path, &stack, 1);
        b = call_function("-print", my_itoa(a), path);
      }
      else
      {
        char *func = pop(&stack);
        a = eval_second_operand(path, &stack, 1);
        b = call_function(func, arg, path);
      }

      int result = compute(postfix[i], a, b);
      stack = push(stack, my_itoa(result));
    }
  }

  free_stack(stack);
  return 0;
}
/**
** \fn int eval(char **postfix)
** \brief Computes the result of the operation given in postfix notation
** \param char **postfix, the list of string representing the operation in
**  postfix
** \return 1 if postfix is evaluated to true, 0 otherwise
*/
/*
int eval(char *path, char **postfix, int len)
{
  struct stack *stack = init();
  char *s = NULL;
  char *arg = NULL;
  char *func = NULL;
  for (int i = 0; i < len; ++i)
  {
    if (!is_operator(postfix[i]))
        stack = push(stack, postfix[i]);
    if (is_operator(postfix[i]))
    {
      int a = 0;
      int b = 0;
      func = pop(&stack);
      if (my_strcmp(func, "0") || my_strcmp(func, "1"))
      {
        a = func[0] - '0';
        arg = pop(&stack);
        func = pop(&stack);
        b = call_function(func, arg, path);
      }
      else if (!my_strcmp(postfix[i], "!"))
      {
        if (my_strcmp(func, "-print"))
        {
          s = pop(&stack);
          if (my_strcmp(s, "0"))
            b = s[0] - '0';
          else if (my_strcmp(s, "1"))
            b = s[0] - '0';
          else if (my_strcmp(s, "-print"))
            b = call_function("-print", "1", path);
          else
          {
            arg = s;
            func = pop(&stack);
            b = call_function(func, arg, path);
          }
          a = call_function("-print", my_itoa(b), path);
        }
        else
        {
          arg = func;
          func = pop(&stack);
          a = call_function(func, arg, path);
          s = pop(&stack);
          if (my_strcmp(s, "0"))
            b = s[0] - '0';
          else if (my_strcmp(s, "1"))
            b = s[0] - '0';
          else
          {
            arg = s;
            func = pop(&stack);
            b = call_function(func, arg, path);
          }
        }
      }
      else
      {
        arg = pop(&stack);
        a = call_function(func, arg, path);
      }
      int result = compute(postfix[i], a, b);
      stack = push(stack, my_itoa(result));
    }
  }
  //free(s);
  //pop(&stack);
  free_stack(stack);
  return 0;
}*/
