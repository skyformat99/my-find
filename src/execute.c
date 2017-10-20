#include <stdlib.h>
#include "utilities.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static char *get_command(char *arg);
static char **get_arg(const char *path, char *command, char *arg, int *bracket);

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

int action_exec(char *arg, char *path)
{
  int bracket = 0;
  char *command = get_command(arg);
  char **new_arg = get_arg(path, command, arg, &bracket);
  pid_t cpid;
  int status;

  cpid = fork();

  if (cpid == 0)
  {
    execvp(command, new_arg);
    /* UNKNOWN COMMAND ERROR MESSAGE HERE */
    exit(0);
  }

  else {
    pid_t tpid = wait(&status);

    while(tpid != cpid)
      tpid = wait(&status);
  }

  if (bracket)
    free(new_arg[1]);
  free(new_arg);
  free(command);
  return 0;
}
