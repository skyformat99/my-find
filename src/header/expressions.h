#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H

void split_string(char *expressions, char ***parsed);
int test_name(const char *pattern, const char *string);
int test_type(const char *file, const char *type);
int call_function(char *func, char *arg, char *path);

#endif
