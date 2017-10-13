/**
 * \file myfind.c
 * \brief Tentavie d'implementation de find(1).
 * \author noel_m
 * \version 0.1
 * \date 10 Octobre 2017
 *
 * Programme permettant de trouver des fichiers dans une architecture.
 *
 */

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "myfind.h"
#include "parse_arg.h"
#include "search.h"

int main(int argc, char *argv[])
{
  int r_val = 0;
  struct argument *arg = init_arg();
  if (!arg)
  {
    printf("mydinf: %s\n", strerror(errno));
    return 1;
  }
  parse_arg(argc, &argv[1], arg);
  char option = get_options(arg);
  r_val = search(arg, option);
  free_arg(arg);
  return r_val != 0;
}

 /**
 ** \fn struct argument *init_arg(void)
 ** \brief Creation of a new instance of an object of type struct argument.
 **
 ** \param None.
 ** \return new instance of type argumznt or NULL if malloc failed.
 */
struct argument *init_arg(void)
{
  struct argument *arg = malloc(sizeof(struct argument));
  struct categories *options = malloc(sizeof(struct categories));
  struct categories *files = malloc(sizeof(struct categories));
  struct categories *expressions = malloc(sizeof(struct categories));
  if (!arg || !options || !files || !expressions)
    return NULL;
  arg->options = options;
  arg->files = files;
  arg->expressions = expressions;
  return arg;
}

 /**
 ** \fn void free_arg(struct argument *arg)
 ** \brief Free the argument structure and its field.
 **
 ** \param struct argument *arg.
 ** \return void.
 */
void free_arg(struct argument *arg)
{
  free(arg->options);
  free(arg->files);
  free(arg->expressions);
  free(arg);
}