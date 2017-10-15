#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H

#include "parse_arg.h"

void format_expr(struct argument *arg);
int call_function(char *func, char *arg, char *path);
int test_type(const char *file, const char *type);

#endif
