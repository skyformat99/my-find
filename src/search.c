/**
** \file search.c
** \brief All the functions related to the file exploration.
*/

#include <dirent.h>
#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "parse_arg.h"
#include "evalexpr.h"
#include "expressions.h"
#include "search.h"
#include "utilities.h"

static int is_symlink(char *path);
static int is_valid_dir(char *path, char **postfix, int len, char option);

/**
** \fn int search(struct argument *arg)
** \brief Search in all the dir given as argument.
**
** \param struct argument *arg, the main argument struct,
** char **postfix, the array of expressions in postfix, int len,
** the len of postfix, char option, the option flag.
** \return 0 if succes, greater then 0 otherwise.
*/
int search(struct argument *arg, char **postfix, int len, char option)
{
  char **files = arg->files->string_array;
  int filelen = arg->files->len;
  int r_val = 0;
  /* search in current folder if no arguments were given */
  if (!files)
  {
    if (!(option & OPT_D))
      eval(".", postfix, len);
    r_val += search_in_dir(".", postfix, len, option);
    if (option & OPT_D)
      eval(".", postfix, len);
  }
  else
  {
    for (int i = 0; i < filelen; i++)
    {
      if (is_valid_dir(files[i], postfix, len, option))
      {
        search_in_dir(files[i], postfix, len, option);
        if (option & OPT_D)
          eval(files[i], postfix, len);
      }
      else
        r_val++;
    }
  }
  return r_val;
}

/**
** \fn is_valid_dir(char *path, char **postfix, int len, char option)
** \brief Check if dir given as arg is valid and print its name.
** \param char *path, the path toward the dir to check,
** char **postfix, the array of expressions in postfix,
**  int len, the len of postfix, char option, the option flag.
** \return 1 if directory is a symlink, 0 otherwise.
*/
static int is_valid_dir(char *path, char **postfix, int len, char option)
{
  DIR *dir = opendir(path);
  if (!dir)
  {
    if (test_type(path, "f"))
    {
      eval(path, postfix, len);
      return 0;
    }
    warnx("‘%s’: No such file or directory", path);
    return 0;
  }
  /* if the dir is a simlink and D or L option not set, we print and stop */
  if (is_symlink(path) && (!(option & OPT_H) && !(option & OPT_L)))
  {
    eval(path, postfix, len);
    return 0;
  }
  if (!(option & OPT_D))
    eval(path, postfix, len);

  closedir(dir);
  return 1;
}

/**
** \fn static int is_symlink(char *path)
** \brief Indicates if dir at path is symbolic link
** \param char *path, path to directory
** \return 1 if directory is a symlink, 0 otherwise.
*/
static int is_symlink(char *path)
{
  struct stat filestat;
  lstat(path,&filestat);

  if (S_ISLNK(filestat.st_mode))
    return 1;

  return 0;
}

/**
** \fn int search_in_dir(char *path)
** \brief Recursively print the content of dir and the content of the
** dir they contain.
** \param char *path, path to directory
** \return 0 if succes, greater then 1 otherwise.
*/
int search_in_dir(char *path, char **postfix, int len, char option)
{

  DIR *dir = opendir(path);
  if (!dir)
    return 1;
  char *subdir = NULL;
  struct dirent *dp = NULL;
  if (path[my_strlen(path) -1] == '/')
      path[my_strlen(path)-1] = '\0';

  while ((dp = readdir(dir)))
  {
    /* recursiveley search in dir if dir is not . or .. */
    if (!my_strcmp(dp->d_name, ".")
        && !my_strcmp(dp->d_name, ".."))
    {
        /* size of the two names + the nul character and the / */
      subdir = calloc(my_strlen(path) + my_strlen(dp->d_name) + 2, 1);
      my_strcat(subdir, path);
      my_strcat(subdir, "/");
      my_strcat(subdir, dp->d_name);

      if (!(option & OPT_D))
        eval(subdir, postfix, len);
      if ((is_symlink(subdir) && option & OPT_L) || dp->d_type & DT_DIR)
          search_in_dir(subdir, postfix, len, option);
      if (option & OPT_D)
        eval(subdir, postfix, len);
      free(subdir);
    }
  }

  closedir(dir);
  return 0;
}
