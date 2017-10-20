#ifndef SEARCH_H
#define SEARCH_H

#include "parse_arg.h"

int search(struct argument *arg, char **postfix, int len, char option);
int search_in_dir(char *path, char **postfix, int len, char option);

#endif
