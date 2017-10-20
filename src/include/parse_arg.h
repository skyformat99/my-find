#ifndef PARSE_ARG_H
#define PARSE_ARG_H

/**
 ** \struct categories
 ** \brief Pointer on array of string and it's length.
 */
struct categories
{
  char **string_array;
  int len;
};

/**
 ** \struct argument
 ** \brief Contains all the different categories of arguments stored in an
 ** categories structure
 */
struct argument
{
  struct categories *options;
  struct categories *files;
  struct categories *expressions;
};

enum options {
  OPT_D = 0x01,
  OPT_H = 0x02,
  OPT_L = 0x04,
  OPT_P = 0x08
};

void parse_arg(int argc, char *argv[], struct argument *arg);
char get_options(struct argument *arg);
void is_valid_expr(struct argument *arg);

#endif
