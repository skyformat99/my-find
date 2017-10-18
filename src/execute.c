#include <stdlib.h>
#include "utilities.h"

static char *get_command(char *arg);
static char **get_arg(const char *path, char *arg);

static char **get_arg(const char *path, char *arg)
{
  if (!arg)
  {
    char **null = calloc(1, sizeof(char *));
    return null;
  }

  int bracket = 0;
  for (int i = 0; arg[i] != '\0'; ++i)
    if (arg[i+1] != '\0' && arg[i] == '{' && arg[i+1] == '}')
      bracket = 1;

  char **new = calloc(2, sizeof(char *));

  if (!bracket)
  {
    new[0] = arg;
    return new;
  }

  char *new_arg = calloc(my_strlen(arg) + my_strlen(path) - 1, 1);

  int j = 0;
  for (int i = 0; arg[i] != '\0'; ++i, j++)
  {
    if (arg[i] == '{')
    {
      for (int y = 0; path[y] != '\0'; ++y, ++j)
        new_arg[j] = path[y];
      i += 2;
    }
    new_arg[j] = arg[i];
  }
  new[0] = new_arg;
  return new;

}

static char *get_command(char *arg)
{
  char *start = arg;
  int i = 0;
  for (; arg[i] != ' '; ++i)
    ;
  *(arg + i) = '\0';

  char *command = calloc(my_strlen(start) + 1, 1);
  my_strcpy(command, start);

  *(arg + i) = ' ';
  return command;
}

int action_exec(char *arg, char *path)
{
  char *command = get_command(arg);
  char **new_arg = get_arg(path, arg + my_strlen(command) + 1);
  (void)new_arg;
  free(new_arg[0]);
  free(new_arg);
  free(command);
  return 1;
}
