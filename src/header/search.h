#ifndef SEARCH_H
#define SEARCH_H

#include "parse_arg.h"

int search(struct argument *arg, char option);
int search_in_dir(char *path, char option);

#endif
