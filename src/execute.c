#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include "utilities.h"
#include "expressions.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static char *get_command(char *arg);
static char **get_arg(const char *path, char *command, char *arg, int *bracket);

/**
** \fn static char **get_arg(const char *path, char *command, char *arg, int *bracket)
** \brief get the argument to pass to exec and replace brackets by path
** \param const char *path, the current path, char *command, the command name,
** char *arg, the argument passed to exec, int *bracket, boolean to indicate if
** bracket were replaced in order to free them later
** \return the mofified argument
*/
static char **get_arg(const char *path, char *command, char *arg, int *bracket)
{
  if (my_strlen(command) == my_strlen(arg))
  {
    char **null = calloc(2, sizeof(char *));
    null[0] = command;
    return null;
  }

  arg += my_strlen(command) + 1;
  *bracket = 0;

  for (int i = 0; arg[i] != '\0'; ++i)
    if (arg[i+1] != '\0' && arg[i] == '{' && arg[i+1] == '}')
      *bracket = 1;

  char **new = calloc(3, sizeof(char *));

  if (!*bracket)
  {
    new[0] = command;
    new[1] = arg;
    return new;
  }

  char *new_arg = calloc(my_strlen(arg) + my_strlen(path) - 1, 1);

  int j = 0;
  for (int i = 0; *(arg + i) != '\0'; ++i, ++j)
  {
    if (*(arg + i) == '{')
    {
      for (int y = 0; path[y] != '\0'; ++y, ++j)
        new_arg[j] = path[y];
      i += 2;
    }
    new_arg[j] = arg[i];
  }
  new[0] = command;
  new[1] = new_arg;
  return new;
}

/**
** \fn static char *get_command(char *arg)
** \brief get the name of the comman exec is suppose to call
** \param char *arg, the argument given to exec
** \return the name of the command to call
*/
static char *get_command(char *arg)
{
  char *start = arg;
  char *command = calloc(my_strlen(start) + 1, 1);

  int i = 0;
  for (; arg[i] != ' '; ++i)
    if (arg[i] == '\0')
    {
      my_strcpy(command, start);
      return command;
    }

  *(arg + i) = '\0';

  my_strcpy(command, start);

  *(arg + i) = ' ';
  return command;
}

static int launch_program(char *command, char *path, char **new_arg)
{
  pid_t cpid;
  int status;
  setbuf(stdout, NULL);
  cpid = fork();

  if (cpid == 0)
  {
    chdir(path);
    execvp(command, new_arg);
    warnx("‘%s’: No such file or directory", command);
    exit(0);
  }

  else {
    pid_t tpid = wait(&status);

    while(tpid != cpid)
      tpid = wait(&status);
  }

  return !status;
}

/**
** \fn int action_exec(char *arg, char *path)
** \brief execute the command given to find via exec
** \param char *arg, the argument given to exec, char *path the current path
** \return true if the command returned 0, false otherwise
*/
int action_exec(char *arg, char *path)
{
  int bracket = 0;
  char *command = get_command(arg);
  char **new_arg = get_arg(path, command, arg, &bracket);

  int rv = launch_program(command, path, new_arg);

  if (bracket)
    free(new_arg[1]);
  free(new_arg);
  free(command);

  return rv;
}

static char *get_current_dir(char *path, int *modified)
{
  if (call_function("-type", "d", path))
    return path;

  *modified = 1;
  char *cur = path + my_strlen(path);

  for (int i = my_strlen(path); path[i] != '/'; --i, cur--)
    ;

  *cur = '\0';
  return path;
}

/**
** \fn int action_execdir(char *arg, char *path)
** \brief execute the command given to find via execdir in the current
** file's directory
** \param char *arg, the argument given to exec, char *path the current path
** \return true if the command returned 0, false otherwise
*/
int action_execdir(char *arg, char *path)
{
  int bracket = 0;
  int modified = 0;

  char *command = get_command(arg);
  get_current_dir(path, &modified);

  char *current_dir = calloc(my_strlen(path) + 3, 1);
  current_dir = my_strcpy(current_dir, path);
  char **new_arg = NULL;
  char *format = NULL;

  if (modified == 0)
  {
    char *cur = current_dir;
    format = calloc(my_strlen(current_dir) + 3, 1);
    for (int i = 0; current_dir[i] != '\0'; ++i)
      if (current_dir[i] == '/' && current_dir[i + 1] != '\0')
        cur = current_dir + i + 1;
    my_strcat(format, "./");
    my_strcat(format, cur);
    new_arg = get_arg(format, command, arg, &bracket);
  }
  else
  {
    int len = my_strlen(path);
    *(path + len - 1) = '.';
    *(path + len ) = '/';
    new_arg = get_arg(path + len - 1,
                      command, arg, &bracket);
  }

  int rd = launch_program(command, current_dir, new_arg);

  if (bracket)
    free(new_arg[1]);
  free(new_arg);
  free(command);
  free(current_dir);
  if (format)
    free(format);

  return rd;
}
