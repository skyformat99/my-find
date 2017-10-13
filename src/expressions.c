#include <stddef.h>
#include <fnmatch.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parse_arg.h"
#include "utilities.h"

int test_arg(const char *file, char **expressions);

int main(void)
{
  test_arg("ntm", "-name test";);
}

int test_arg(const char *file, char *expressions)
{
  (void)file;
  /* we split the different expressions in a list */
  char *end = expressions;
  char *start = expressions;
  for (; *start != '\0'; start++)
  {
    while (*end != ' ' || *end = '\0')
      end++;

  }

  char** parsed = malloc(sizeof(char *) * size);
  int j = 0;
  for (int i = 0; i < size; ++i, j = my_strlen(&expression[i]) + 1)
    parsed[i] = expression + j;

  for (int i = 0; i < size; ++i)
    printf("%s", parsed[i]);
  return 0;
}
