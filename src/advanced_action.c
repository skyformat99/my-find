/**
** \file myfind.c
** \brief light implementation of find(1).
*/
#include <unistd.h>

int action_delete(char *arg, char *path)
{
  int rv = 1;
  if (arg[0])
    rv = rmdir(path);

  return !rv;
}
