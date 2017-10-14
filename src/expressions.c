#include <err.h>
#include <stddef.h>
#include <fnmatch.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "expressions.h"
#include "utilities.h"
#include "evalexpr.h"
#include "parse_arg.h"

 /**
 ** \fn void append_and(struct argument *arg);
 ** \brief Take the list of expressions, append -print at the end if not
 ** already passed as arg. Append -a operator if no specified
 ** \param struct argument *arg, pointer on the main argument structure
 ** \return void.
 */

void append_and(struct argument *arg)
{
  size_t size = 0;
  int print = 0;
  char **expressions = arg->expressions->string_array;
  for (int i = 0; i < arg->expressions->len; ++i, size++)
  {
    if (my_strcmp(expressions[i], "-print"))
    {
      size--;
      print = 1;
    }
    if (my_strcmp(expressions[i], "-name")
        || my_strcmp(expressions[i], "-type"))
    {
      if (i + 2 < arg->expressions->len && !is_operator(expressions[i+2]))
        size++;
    }
  }

  char **new = malloc(sizeof(char *) * size + 2);

  int y = 0;
  for (int i = 0; i < arg->expressions->len; ++i, ++y)
  {
    new[y] = expressions[i];
    if (my_strcmp(expressions[i], "-name")
        || my_strcmp(expressions[i], "-type"))
    {
      if (i + 2 < arg->expressions->len && !is_operator(expressions[i+2]))
      {
        new[y+1] = expressions[i+1];
        new[y+2] = "-a";
        y += 2;
        i++;
      }
    }
  }
  if (!print)
  {
    if (arg->expressions->len != 0)
    {
      new[y] = "-a";
      new[y + 1] = "-print";
    }
    else
      new[size] = "-print";
  }

  arg->expressions->string_array = new;
  arg->expressions->len = size + (size == 0 ? 1 : 2);
}

int call_function(char *func, char *arg, char *path)
{
  if (my_strcmp(func, "-name"))
    return test_name(path, arg);
  if (my_strcmp(func, "-type"))
    return test_type(path, arg);
  else
    errx(1, "unknown predicate `%s'", func);
}

int test_name(const char *pattern, const char *string)
{
  if (!fnmatch(pattern, string, FNM_PATHNAME)) //replace by fn_file_name
    return 1;
  return 0;
}

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
