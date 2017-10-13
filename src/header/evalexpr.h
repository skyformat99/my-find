#ifndef EVALEXPR_H
#define EVALEXPR_H

#include <stddef.h>

int read_input(char *input, size_t size);
void to_postfix(char **input, char **postfix);
int is_operator(char *s);
int is_priority(char *data, char *input);

#endif
