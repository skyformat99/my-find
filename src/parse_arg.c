#include <err.h>
#include <stdlib.h>
#include "parse_arg.h"
#include "utilities.h"

/**
** \fn void parse_arg(int argc, char *argv[], struct argument *arg)
** \brief Parse the different arguments and store them inside argument.
**
** \param int argc number of parameter, char *argv[] argument,
** struct argument *arg pointer on the argument structure
** \return void.
*/
void parse_arg(int argc, char *argv[], struct argument *arg)
{
  int optlen = 0;
  int filelen = 0;
  int exprelen = 0;

  char **options = NULL;
  char **files = NULL;
  char **expressions = NULL;

  for (int i = 0; i < argc - 1; ++i, ++argv)
  {
    if ((**argv == '-' || **argv == '(') && *(*argv + 1) != 'd'
        && *(*argv + 1) != 'H' && *(*argv + 1) != 'L' && *(*argv + 1) != 'P')
    {
        if (!expressions)
          expressions = argv;
        exprelen++;
    }

    else if (**argv == '-')
    {
      if (!expressions && !options)
        options = argv;
      optlen++;
    }

    if (**argv != '-')
    {
      if (!files && !expressions)
        files = argv;
      if (!expressions)
        filelen++;
      if (expressions && i != 0)
        exprelen++;
    }
  }

  arg->options->string_array = options;
  arg->options->len = optlen;
  arg->files->string_array = files;
  arg->files->len = filelen;
  arg->expressions->string_array = expressions;
  arg->expressions->len = exprelen;
}
/**
** \fn char get_options(struct argument *arg)
** \brief get the different options and sets the flags.
**
** \param a pointer on an argument structure
** \return a char with all the flag sets.
*/
char get_options(struct argument *arg)
{
  char option = '\0';
  if (!arg->options->string_array)
    return option;
  char **options = arg->options->string_array;
  int len = arg->options->len;
  for (int i = 0; i < len; ++i)
  {
    if (my_strcmp("-d", options[i]))
      option |= OPT_D;
    else if (my_strcmp("-H", options[i]))
      option |= OPT_H;
    else if (my_strcmp("-L", options[i]))
      option |= OPT_L;
    else if (my_strcmp("-P", options[i]))
      option |= OPT_P;
    else
      errx(1, "unknown predicate `%s'", options[i]);
  }
  return option;
}
/**
** \fn char get_options(struct argument *arg)
** \brief get the different options and sets the flags.
**
** \param a pointer on an argument structure
** \return a char with all the flag sets.
*/
void is_valid_expr(struct argument *arg)
{
  char **expressions = arg->expressions->string_array;

  for (int i = 0; i < arg->expressions->len; ++i)
  {
    if (expressions[i][0] == '-')
    {
      if (my_strcmp(expressions[i], "-name")
          || my_strcmp(expressions[i], "-type")) 
      {
        if (i + 1 >= arg->expressions->len)
          errx(1, "missing argument to `%s'", expressions[i]);
        ++i;
        continue;
      }
      if (my_strcmp(expressions[i], "-exec")
          || my_strcmp(expressions[i], "-execdir"))
      {
        int j = i;
        for (; j < arg->expressions->len
                             && expressions[j][0] != ';'; j++)
          ;
        if (j >= arg->expressions->len)
          errx(1, "missing argument to `%s'", expressions[i]);
        i = j + 1;
        continue;
      }
      if (my_strcmp(expressions[i], "-print"))
      {
        ++i;
        continue;
      }

      if (my_strcmp(expressions[i], "-o")
          || my_strcmp(expressions[i], "-a"))
      {
        if (i + 1 >= arg->expressions->len)
          errx(1, "expected an expression after '%s'", expressions[i]);
      }
      else
        errx(1, "unknown predicate `%s'", expressions[i]);
    }
  }
}
