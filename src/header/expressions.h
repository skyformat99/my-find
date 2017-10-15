#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H

#include "parse_arg.h"

int test_name(const char *pattern, const char *string);
int test_type(const char *file, const char *type);
int call_function(char *func, char *arg, char *path);
int print(const char* path, int eval);
void append_and(struct argument *arg);

#endif
