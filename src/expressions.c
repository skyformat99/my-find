#include <stddef.h>
#include <fnmatch.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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
