#include <err.h>
#include <stddef.h>
#include <fnmatch.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include "expressions.h"
#include "utilities.h"
#include "evalexpr.h"
#include "execute.h"
#include "parse_arg.h"

static int get_lenformat(char **expressions, int len, int *print);
static int test_name(const char *pattern, char *string);
static int print(const char* path, int eval);
static char* format_exec(char **exec);
static void empty_expr(struct argument **arg);

/**
** \fn static void empty_expr(struct argument **arg);
** \brief if no expressions are specified, adds -name * -a -print
** \param a pointer of pointer on the argument structure
** \return void.
*/
static void empty_expr(struct argument **arg)
{
  char *empty[] = {
    "-name", "*", "-a", "-print"
  };
  char **new = calloc(4, sizeof(char *));
  for (int i = 0; i < 4; ++i)
    new[i] = empty[i];
  (*arg)->expressions->len = 4;
  (*arg)->expressions->string_array = new;
}

/**
** \fn static void not_print(int size, char **new);
** \brief adds parenthesis and print if print not present in initial expr
** \param int size the size of the new expression, char **new an array of string
** \return void.
*/
static void not_print(int size, char **new)
{
    new[0] = "(";
    new[size - 3] = ")";
    new[size - 2] = "-a";
    new[size - 1] = "-print";
}

/**
** \fn void append_and(struct argument *arg);
** \brief Take the list of expressions, append -print at the end if not
** already passed as arg, put's parenthentesis around expressions .
** before -a print. Append -a operator if no specified
** \param struct argument *arg, pointer on the main argument structure
** \return void.
*/
void format_expr(struct argument *arg)
{
  int print = 0;
  int exec = 0;
  int y = 0;
  int len = arg->expressions->len;
  char **expressions = arg->expressions->string_array;
  
  if (len == 0 || (len == 1 && my_strcmp(expressions[0], "-print")))
  {
    empty_expr(&arg);
    return;
  }

  int size = get_lenformat(expressions, arg->expressions->len, &print);

  char **new = calloc(size, sizeof(char *));

  if (!print)
  {
    not_print(size, new);
    y++;
  }
  for (int i = 0; i < arg->expressions->len && expressions[i]; ++i, ++y)
  {
    new[y] = expressions[i];
    if (my_strcmp(expressions[i], "-exec")
     || my_strcmp(expressions[i], "-ecedir"))
     {
       exec = 1;
       if (i == 0)
         size -= 4;
       i++;
       new[++y] = format_exec(expressions + i);
       if (i + 2 < arg->expressions->len && !is_operator(new[y + 1])
           && !my_strcmp(new[y + 1], ")"))
           new[++y] = "-a";
       for (; expressions[i][0] != ';'; ++i)
         ;
     }
    if (my_strcmp(expressions[i], "-name")
        || my_strcmp(expressions[i], "-type"))
    {
      if (i + 2 < arg->expressions->len && !is_operator(expressions[i+2])
          && !my_strcmp(expressions[i+2], ")"))
      {
        new[y+1] = expressions[i+1];
        new[y+2] = "-a";
        y += 2;
        i++;
      }
    }
    if (my_strcmp(expressions[i], "-print"))
      if (i + 1 < arg->expressions->len && !is_operator(expressions[i+1]))
      {
        new[y+1] = "-a";
        y++;
      }
  }
  
  if (exec)
    arg->expressions->string_array = new + 1;
  else
    arg->expressions->string_array = new;
  arg->expressions->len = size;
}

/**
** \fn static char* format_exec(char **expressions)
** \brief creates a new string with all the argument to exec
** concatenated in one string seperate by space
** \param char **expressions, the start of the argument to exec
** \return the new string.
*/
static char* format_exec(char **expressions)
{
  int len = 0;
  int nb = -1;
  for (int j = 0; expressions[j][0] != ';'; ++j)
  {
    len += my_strlen(expressions[j]);
    nb++;
  }
  /** the new string size equals to size of string
   ** sperated by space + null byte
   */
  char *exec = calloc(len + nb + 2, 1);
  for (int i = 0; expressions[i][0] != ';'; ++i, --nb)
  {
    my_strcat(exec, expressions[i]);
    if (nb > 0)
      my_strcat(exec, " ");
  }
  return exec;
}

/**
** \fn static int get_lenformat(char **expressions, int len, int *print)
** \brief Take the list of expressions, computes the length needed
** for the formatted string
** \param char **expressions, the expression in prefix order, int len it's len
** \return the size need for the formatted expression.
*/
static int get_lenformat(char **expressions, int len, int *print)
{
  int size = len;
  int second = 0;
  for (int i = 0; i < len; ++i)
  {
    if (my_strcmp(expressions[i], "-exec")
        || my_strcmp(expressions[i], "-execdir"))
    {
      i++;
      if (second)
        size++;
      for (; expressions[i][0] != ';'; ++i)
        size--;
      second = 1;
    }
    if (my_strcmp(expressions[i], "-name")
        || my_strcmp(expressions[i], "-type")
        || my_strcmp(expressions[i], "-exec")
        || my_strcmp(expressions[i], "-execdir"))
      if (i + 2 < len && !is_operator(expressions[i+2])
          && !my_strcmp(expressions[i+2], ")"))
        size++;

    if (my_strcmp(expressions[i], "-print"))
      if (i + 1 < len && !is_operator(expressions[i+1]))
        size++;
    if (my_strcmp(expressions[i], "-print"))
      *print = 1;
  }

  size += *print ? 0 : 4;
  return size;
}

/**
** \fn int call_function(char *func, char *arg, char *path)
** \brief Call the function given as a string with argument and path.
** \param char *func, the function to call, char *arg, its argument,
** char *path, th epath on which to make the test.
** \return the return value of the funcion called
*/
int call_function(char *func, char *arg, char *path)
{
  if (my_strcmp(func, "-name"))
    return test_name(arg, path);
  if (my_strcmp(func, "-type"))
    return test_type(path, arg);
  if (my_strcmp(func, "-print"))
    return print(path, arg[0] - '0');
  if (my_strcmp(func, "-exec"))
    return action_exec(arg, path);
  else
    errx(1, "unknown predicate `%s'", func);
}

/**
** \fn static int test_name(const char *pattern, const char *string)
** \brief test if pattern matches string.
** \param const char *pattern, the pattern, const char *string, the name to
** test.
** \return 1 if pattern matches string, zero otherwise
*/
static int test_name(const char *pattern, char *string)
{
  char *cur = string;

  for (int i = 0; string[i] != '\0'; i++)
    if (string[i] == '/')
        cur = string + i;

  if (!fnmatch(pattern, cur+1, 0))
    return 1;
  return 0;
}

/**
** \fn static int test_type(const char *file, const char *type)
** \brief test if file is of type type.
** \param const char *file, the path to file, const char *type, the type we
** want to test.
** \return 1 if file is of type refered by type
*/
int test_type(const char *file, const char *type)
{
  struct stat filestat;
  lstat(file,&filestat);

  if (my_strcmp(type, "b"))
    return S_ISBLK(filestat.st_mode);
  if (my_strcmp(type, "c"))
    return S_ISCHR(filestat.st_mode);
  if (my_strcmp(type, "d"))
    return S_ISDIR(filestat.st_mode);
  if (my_strcmp(type, "f"))
    return S_ISREG(filestat.st_mode);
  if (my_strcmp(type, "l"))
    return S_ISLNK(filestat.st_mode);
  if (my_strcmp(type, "p"))
    return S_ISFIFO(filestat.st_mode);
  if (my_strcmp(type, "s"))
    return S_ISSOCK(filestat.st_mode);
  else
    errx(1, "Unknown argument to -type: %s", type);
}

/**
** \fn static int print(const char* path, int eval)
** \brief print path if eval is true
** \param const char* path, the path, int eval, boolean
** \return 1
*/
static int print(const char* path, int eval)
{
  if (eval)
    printf("%s\n", path);
  return 1;
}
