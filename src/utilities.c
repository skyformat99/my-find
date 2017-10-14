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

/**
** \fn int my_strlen(const char *s1)
** \brief computes the length of a string
** \return the length of the string.
*/
int my_strlen(const char *s1)
{
  int i = 0;
  if (s1)
    while (s1[i] != '\0')
      i++;
  return i;
}

/**
** \fn char *my_strcat(char *s1, const char *s2)
** \brief concatenates string
** \return the new string, result of the concatenation of s1 and s2.
*/
char *my_strcat(char *s1, const char *s2)
{
  int i = my_strlen(s1);
  for (int j = 0; s2[j] != '\0'; ++i, ++j)
    s1[i] = s2[j];

  return s1;
}
