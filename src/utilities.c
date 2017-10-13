#include "utilities.h"

/**
** \fn my_strcmp(const char *s1, const char *s2)
** \brief Compares to string.
** \return 1 if string similar 0 otherwise.
*/
int my_strcmp(const char *s1, const char *s2)
{
  int i = 0;
  for (; s1[i] != '\0'; ++i)
    if (s1[i] != s2[i])
      return 0;
  return s2[i] == '\0';
}

int my_strlen(const char *s1)
{
  int i = 0;
  while (s1[i] != '\0')
    i++;
  return i;
}

char *my_strcat(char *s1, const char *s2)
{
  int i = my_strlen(s1);
  for (int j = 0; s2[j] != '\0'; ++i, ++j)
    s1[i] = s2[j];

  return s1;
}
