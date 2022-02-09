#ifndef SRC_SMARTCALC_H_
#define SRC_SMARTCALC_H_

#include <stdio.h>

#define SIZE 256
#define EPS 1e-8

#define OPERANDS {"^", "%", "*", "/", "+", "-"}
#define FUNCTIONS {"cos", "sin", "tan", "acos", "asin", "atan", "sqrt", "ln", "log"}

struct type_stack {
    char **stack;
    size_t point;
    size_t flag_op;
    size_t flag_func;
    size_t flag_x;
    size_t flag_minus;
    size_t error;
};

struct type_double {
    double *array;
    size_t size;
    size_t point;
    size_t error;
};

struct flags {
    size_t ops;
    size_t nums;
    size_t funcs;
};

void create_double(struct type_double *c);
void realloc_double(struct type_double *c);
void destroy_double(struct type_double *c);
void push_double(struct type_double *c, double d);
double pop_double(struct type_double *c);
void init_stack(struct type_stack *a);
void destroy_stack(struct type_stack *a);
void push_stack(char *input, struct type_stack *a);
void pop_stack(char *str, struct type_stack *a);
void init_flags(struct flags *a);
void make_space_out(char *out);
void make_space_in(char *in);
int check_parenthesis(struct type_stack *a);
void check_flags(char *token, struct type_stack *a);
void check_operands(char *token, char *out, struct type_stack *a);
void sort(char *in, char *out, struct type_stack *a, struct flags *errors);
double my_atof(char *s);
void parser_func(char *token, struct type_double *c);
double parser(double x, char *out, struct type_double *c);

#endif  // SRC_SMARTCALC_H_
