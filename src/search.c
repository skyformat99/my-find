#include <dirent.h>
#include <errno.h>
#include <err.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "parse_arg.h"
#include "search.h"
#include "utilities.h"

static int is_valid_dir(char *path, char option);
/**
** \fn int search(struct argument *arg)
** \brief Search in all the dir given as argument.
**
** \param struct argument *arg
** \return 0 if succes, greater then 0 otherwise.
*/
int search(struct argument *arg, char option)
{
  char **files = arg->files->string_array;
  int filelen = arg->files->len;
  int r_val = 0;
  /* search in current folder if no arguments were given */
  if (!files)
  {
    if (!(option & OPT_D))
      printf(".\n");
    r_val += search_in_dir(".", option);
    if (option & OPT_D)
      printf(".\n");
  }
  else
  {
    for (int i = 0; i < filelen; i++)
    {
      if (is_valid_dir(files[i], option))
      {
        search_in_dir(files[i], option);
        if (option & OPT_D)
          printf("%s\n", files[i]);
      }
      else
        r_val++;
    }
  }
  return r_val;
}

static int is_valid_dir(char *path, char option)
{
  DIR *dir = opendir(path);
  if (!dir)
  {
    warnx("‘%s’: No such file or directory", path);
    return 0;
  }
  if (!(option & OPT_D))
    printf("%s\n", path);
  closedir(dir);
  return 1;
}

/**
** \fn int search_in_dir(char *path)
** \brief Recursively print the content of dir and the content of the
** dir they contain.
** \param char *path, path to directory
** \return 0 if succes, greater then 1 otherwise.
*/
int search_in_dir(char *path, char option)
{
  
  DIR *dir = opendir(path);
  char *subdir = NULL;
  struct dirent *dp = NULL;
  if (path[strlen(path) -1] == '/')
      path[strlen(path)-1] = '\0';
  
  while ((dp = readdir(dir)))
  {
    /* recursiveley search in dir if dir is not . or .. */
    if (!my_strcmp(dp->d_name, ".")
        && !my_strcmp(dp->d_name, ".."))
    {
      if (!(option & OPT_D))
        printf("%s/%s\n", path, dp->d_name);
      if (dp->d_type & DT_DIR)
      {
        /* size of the two names + the nul character and the / */
        subdir = calloc(my_strlen(path) + my_strlen(dp->d_name) + 2, 1);
        my_strcat(subdir, path);
        my_strcat(subdir, "/");
        my_strcat(subdir, dp->d_name);
        if ((!(dp->d_type & DT_LNK)) || option & OPT_L)
            search_in_dir(subdir, option);
        free(subdir);
      }
      if (option & OPT_D)
        printf("%s/%s\n", path, dp->d_name);
    }
  }

  closedir(dir);
  return 0;
}
