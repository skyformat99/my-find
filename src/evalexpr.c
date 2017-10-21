#include "stack.h"
#include "evalexpr.h"
#include "utilities.h"
#include "expressions.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

static int eval_second_operand(char *path, struct stack **stack, int a);
static int should_eval(int a, char *operator);

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

/**
** \fn char *my_itoa(int a)
** \brief returns the int a in a string (always 0 or 1)
** \param int a, the int to convert to string
** \return the new string
*/
char *my_itoa(int a)
{
  if (a == 0)
    return "0";
  return "1";
}

/**
** \fb static int eval_second_operand(char *path, struct stack **st, int a)
** \brief evaluate the second operand
** \param char *path, the path, struct stack **st, a pointer on the stack,
** the value returned by the last operand
** \return the value of the second operand
*/
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

/**
** \fn static int should_eval(int a, char *operator)
** \brief function to determine if the next operand should be evaluated
** \param int a, the previous operand, char *operator the operator
** \return true if the last operatand is 0 and operator is OR and true if
** last operand is 1 and operator is AND
*/
static int should_eval(int a, char *operator)
{
 return ((a == 0 && !my_strcmp(operator, "-a"))
          || (a == 1 && (!my_strcmp(operator, "-o"))));
}

/**
** \fn int eval(char **postfix)
** \brief Computes the result of the operation given in postfix notation
** \param char **postfix, the list of string representing the operation in
**  postfix
** \return 1 if postfix is evaluated to true, 0 otherwise
*/
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
        if (should_eval(a, postfix[i]))
          b = call_function("-print", my_itoa(a), path);
      }
      else
      {
        char *func = pop(&stack);
        a = eval_second_operand(path, &stack, 1);
        if (should_eval(a, postfix[i]))
          b = call_function(func, arg, path);
      }

      int result = compute(postfix[i], a, b);
      stack = push(stack, my_itoa(result));
    }
  }

  free_stack(stack);
  return 0;
}
