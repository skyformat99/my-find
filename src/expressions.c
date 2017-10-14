#include <err.h>
#include <stddef.h>
#include <fnmatch.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "expressions.h"
#include "utilities.h"

 /**
 ** \fn void split_string(char *expressions, char **parsed);
 ** \brief Take a string and split it in a list of string separated by space
 ** \param char ***arg pointer on the string of string
 ** \return void.
 */

void split_string(char *expressions, char ***parsed)
{
  /* we split the different expressions in a list */
  char *start = expressions;
  char *end = expressions;

  int i = 0;
  size_t size = 0;
  size_t len = 1;

  for (int i = 0; expressions[i] != '\0'; i++)
    if (expressions[i] == ' ')
      len++;

  char **new = malloc(sizeof(char *) * len + 1);

  while (*start != '\0')
  {
    while (*end != ' ' && *end != '\0')
    {
      size++;
      end++;
    }

    char *cmd = malloc(size+1);
    memcpy(cmd, start, size);
    cmd[size] = '\0';
    new[i] = cmd;
    i++;
    start = (end + 1);
    end++;
    size = 0;
  }

  new[len] = NULL;
  *parsed = new;
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
