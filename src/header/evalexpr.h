#ifndef EVALEXPR_H
#define EVALEXPR_H

#include <stddef.h>

void to_postfix(char **input, int len, char **postfix);
int is_operator(char *s);
int is_priority(char *data, char *input);
char *my_itoa(int a);
int eval(char *path, char **postfix);

#endif
