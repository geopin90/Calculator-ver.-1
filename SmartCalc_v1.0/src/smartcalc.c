#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "smartcalc.h"

void create_double(struct type_double *c) {
    c->size = SIZE;
    c->point = 0;
    c->error = 0;
    c->array = (double*)malloc(c->size * sizeof(double));
}

void realloc_double(struct type_double *c) {
    c->size *= 2;
    c->array = (double*)realloc(c->array, c->size * sizeof(double));
}

void destroy_double(struct type_double *c) {
    c->size = 0;
    c->point = 0;
    c->error = 0;
    free(c->array);
}

void push_double(struct type_double *c, double d) {
    if (c->point == c->size)
        realloc_double(c);
    c->array[c->point] = d;
    c->point++;
}

double pop_double(struct type_double *c) {
    double d = 0.0;
    if (c->point > 0) {
        c->point--;
        d = c->array[c->point];
    }
    return d;
}

void init_stack(struct type_stack *a) {
    a->point = 0;
    a->flag_op = 0;
    a->flag_func = 0;
    a->flag_x = 0;
    a->flag_minus = 0;
    a->error = 0;
    a->stack = (char**)malloc(SIZE * sizeof(char*));
    for (int i = 0; i < SIZE; i++)
        a->stack[i] = (char*)malloc(SIZE * sizeof(char));
}

void destroy_stack(struct type_stack *a) {
    a->point = 0;
    a->flag_op = 0;
    a->flag_func = 0;
    a->flag_x = 0;
    a->flag_minus = 0;
    a->error = 0;
    for (int i = 0; i < SIZE; i++)
        free(a->stack[i]);
    free(a->stack);
}

void push_stack(char *input, struct type_stack *a) {
    snprintf(a->stack[a->point], SIZE, "%s", input);
    a->point++;
}

void pop_stack(char *str, struct type_stack *a) {
    if (a->point > 0) {
        a->point--;
        snprintf(str, SIZE, "%s", a->stack[a->point]);
    }
}

void init_flags(struct flags *a) {
    a->ops = 0;
    a->nums = 0;
    a->funcs = 0;
}

void make_space_out(char *out) {
    char buf[SIZE] = {};
    size_t len = strlen(out), count = 0;
    for (size_t i = 0; i < len; i++) {
        if ((out[i] == '*' || out[i] == '/' || out[i] == '+') && out[i-1] != ' ') {
            buf[count++] = ' ';
        } else if ((out[i-1] == '*' || out[i-1] == '/' || out[i-1] == '+') && out[i] != ' ') {
            buf[count++] = ' ';
        } else if (out[i] >= '0' && out[i] <= '9' && out[i-1] == '-') {
            buf[count++] = out[i++];
        } else if ((out[i] == '-' || out[i] == '^' || out[i] == '%') && out[i-1] != ' ') {
            buf[count++] = ' ';
        } else if ((out[i-1] == '-' || out[i-1] == '^' || out[i-1] == '%') && out[i] != ' ') {
            buf[count++] = ' ';
        } else if ((out[i] == '(' || out[i] == ')') && out[i-1] != ' ') {
            buf[count++] = ' ';
        } else if ((out[i-1] == '(' || out[i-1] == ')') && out[i] != ' ') {
            buf[count++] = ' ';
        }
        buf[count++] = out[i];
    }
    buf[count] = '\0';
    if (count > 0)
        snprintf(out, SIZE, "%s", buf);
}

void make_space_in(char *in) {
    char buf[SIZE] = {};
    int flag = 0;
    size_t len = strlen(in), count = 0;
    for (size_t i = 0; i < len; i++) {
        if (in[i-1] == '(' && in[i] == '+') {
            continue;
        } else if ((in[i] == '*' || in[i] == '/' || in[i] == '+') && in[i-1] != ' ') {
            buf[count++] = ' ';
        } else if ((in[i-1] == '*' || in[i-1] == '/' || in[i-1] == '+') && in[i] != ' ') {
            buf[count++] = ' ';
        } else if (in[i-1] == '(' && in[i] == '-') {
            buf[count++] = ' ';
            flag++;
        } else if (in[i] >= '0' && in[i] <= '9' && in[i-1] == '-' && flag) {
            flag--;
        } else if ((in[i] == '-' || in[i] == '^' || in[i] == '%') && in[i-1] != ' ') {
            buf[count++] = ' ';
        } else if ((in[i-1] == '-' || in[i-1] == '^' || in[i-1] == '%') && in[i] != ' ') {
            buf[count++] = ' ';
        } else if ((in[i] == '(' || in[i] == ')') && in[i-1] != ' ') {
            buf[count++] = ' ';
        } else if ((in[i-1] == '(' || in[i-1] == ')') && in[i] != ' ') {
            buf[count++] = ' ';
        }
        buf[count++] = in[i];
    }
    buf[count] = '\0';
    if (count > 0)
        snprintf(in, SIZE, "%s", buf);
}

int check_parenthesis(struct type_stack *a) {
    int check = 0;
    for (size_t i = 0; i < a->point; i++) {
        if ((strcmp(a->stack[i], "(")) == 0)
            check = 1;
    }
    return check;
}

void check_flags(char *token, struct type_stack *a) {
    static char *oper[] = OPERANDS, *func[] = FUNCTIONS;
    size_t i = 0;
    for (; i < 6; i++) {
        if ((strcmp(token, oper[i])) == 0)
            a->flag_op = 1;
    }
    for (i = 0; i < 9; i++) {
        if ((strcmp(token, func[i])) == 0)
            a->flag_func = 1;
    }
    if ((strcmp(token, "x")) == 0)
        a->flag_x = 1;
}

void check_operands(char *token, char *out, struct type_stack *a) {
    char *buf = (char*)malloc(SIZE * sizeof(char));
    if (token[0] == '%') {
        if (a->point > 0 && (a->stack[a->point - 1][0] == '^' || a->stack[a->point - 1][0] == '%')) {
            pop_stack(buf, a);
            strncat(out, buf, SIZE);
        }
        push_stack(token, a);
    } else if (token[0] == '^') {
        push_stack(token, a);
    } else if (token[0] == '*' || token[0] == '/') {
        if (a->point > 0 && a->stack[a->point - 1][0] == '^') {
            while (a->point > 0 && a->stack[a->point - 1][0] == '^') {
                pop_stack(buf, a);
                strncat(out, buf, SIZE);
            }
        } else if (a->point > 0 && (a->stack[a->point - 1][0] == '%')) {
            pop_stack(buf, a);
            strncat(out, buf, SIZE);
        } else if (a->point > 0 && (a->stack[a->point - 1][0] == '*' || a->stack[a->point - 1][0] == '/')) {
            pop_stack(buf, a);
            strncat(out, buf, SIZE);
        }
        push_stack(token, a);
    } else if (token[0] == '+' || (token[0] == '-')) {
        if (a->point > 0 && a->stack[a->point - 1][0] == '^') {
            while (a->point > 0 && a->stack[a->point - 1][0] == '^') {
                pop_stack(buf, a);
                strncat(out, buf, SIZE);
            }
        } else if (a->point > 0 && (a->stack[a->point - 1][0] == '%')) {
            pop_stack(buf, a);
            strncat(out, buf, SIZE);
        } else if (a->point > 0 && (a->stack[a->point - 1][0] == '*' || a->stack[a->point - 1][0] == '/')) {
            pop_stack(buf, a);
            strncat(out, buf, SIZE);
        } else if (a->point > 0 && (a->stack[a->point - 1][0] == '+' || a->stack[a->point - 1][0] == '-')) {
            pop_stack(buf, a);
            strncat(out, buf, SIZE);
        }
        push_stack(token, a);
    }
    free(buf);
}

void sort(char *in, char *out, struct type_stack *a, struct flags *errors) {
    make_space_in(in);
    char *buf = (char*)malloc(SIZE * sizeof(char));
    char space[] = " ", *token = NULL, *saveptr = NULL;
    int parenthesis = 0;
    for (token = strtok_r(in, space, &saveptr); token; token = strtok_r(NULL, space, &saveptr)) {
        a->flag_op = 0, a->flag_func = 0;
        check_flags(token, a);
        if ((token[0] >= '0' && token[0] <= '9') || (token[0] == '-' && token[1] != '\0')
        || token[0] == '.' || token[0] == 'x' || token[0] == 'e') {
            errors->nums++;
            if (out[0] != '\0') strncat(out, " ", SIZE);
            strncat(out, token, SIZE);
        } else if (a->flag_op) {
            errors->ops++;
            check_operands(token, out, a);
        } else if (a->flag_func) {
            errors->funcs++;
            push_stack(token, a);
        } else if (token[0] == '(') {
            push_stack(token, a);
            parenthesis++;
        } else if (token[0] == ')') {
            if (check_parenthesis(a)) {
                if (out[strlen(out)-1] != ' ') strncat(out, " ", SIZE);
                while (a->stack[a->point-1][0] != '(') {
                    pop_stack(buf, a);
                    strncat(out, buf, SIZE);
                }
                pop_stack(buf, a);
                parenthesis--;
                if (a->point) {
                    check_flags(a->stack[a->point-1], a);
                    if (a->flag_func) {
                        pop_stack(buf, a);
                        strncat(out, buf, SIZE);
                    }
                }
            } else {
                a->error = 1;
            }
        }
    }
    if (parenthesis) { a->error = 1; }
    while (a->point > 0) {
        if (out[strlen(out)-1] != ' ') strncat(out, " ", SIZE);
        pop_stack(buf, a);
        strncat(out, buf, SIZE);
    }
    out[strlen(out)] = '\0';
    make_space_out(out);
    free(buf);
}

double my_atof(char *s) {
    double a = 0.0;
    int e = 0;
    int c;
    int minus = 1;
    while (((c = *s++) != '\0' && c >= '0' && c <= '9') || c == '-') {
        if (c == '-') {
            minus *= -1;
        } else {
            a = a*10.0 + (c - '0');
        }
    }
    if (c == '.') {
        while ((c = *s++) != '\0' && c >= '0' && c <= '9') {
            a = a*10.0 + (c - '0');
            e = e-1;
        }
    }
    if (c == 'e' || c == 'E') {
        int sign = 1;
        int i = 0;
        c = *s++;
        if (c == '+') {
            c = *s++;
        } else if (c == '-') {
            c = *s++;
            sign = -1;
        }
        while (c >= '0' && c <= '9') {
            i = i*10 + (c - '0');
            c = *s++;
        }
        e += i*sign;
    }
    while (e > 0) {
        a *= 10.0;
        e--;
    }
    while (e < 0) {
        a *= 0.1;
        e++;
    }
    return a * minus;
}

void parser_func(char *token, struct type_double *c) {
    double a = pop_double(c);
    if (strcmp(token, "cos") == 0) {
        push_double(c, cos(a));
    } else if (strcmp(token, "sin") == 0) {
        push_double(c, sin(a));
    } else if (strcmp(token, "tan") == 0) {
        push_double(c, tan(a));
    } else if (strcmp(token, "acos") == 0) {
        push_double(c, acos(a));
    } else if (strcmp(token, "asin") == 0) {
        push_double(c, asin(a));
    } else if (strcmp(token, "atan") == 0) {
        push_double(c, atan(a));
    } else if (strcmp(token, "sqrt") == 0) {
        push_double(c, sqrt(a));
    } else if (strcmp(token, "ln") == 0) {
        push_double(c, log(a));
    } else if (strcmp(token, "log") == 0) {
        push_double(c, log10(a));
    }
}

double parser(double x, char *out, struct type_double *c) {
    const char space[] = " ";
    char *token = NULL, *saveptr = NULL;
    double a, b;
    for (token = strtok_r(out, space, &saveptr); token; token = strtok_r(NULL, space, &saveptr)) {
        if ((token[0] >= '0' && token[0] <= '9') || token[0] == '.'
        || (token[0] == '-' && token[1] != '\0')) {
            push_double(c, my_atof(token));
        } else if (token[0] == 'x') {
            push_double(c, x);
        } else if (token[0] == '+') {
            b = pop_double(c);
            a = pop_double(c);
            push_double(c, (a + b));
        } else if (token[0] == '-') {
            b = pop_double(c);
            a = pop_double(c);
            push_double(c, (a - b));
        } else if (token[0] == '*') {
            b = pop_double(c);
            a = pop_double(c);
            push_double(c, (a * b));
        } else if (token[0] == '/') {
            b = pop_double(c);
            a = pop_double(c);
            push_double(c, (a / b));
        } else if (token[0] == '^') {
            b = pop_double(c);
            a = pop_double(c);
            push_double(c, (pow(a, b)));
        } else if (token[0] == '%') {
            b = pop_double(c);
            a = pop_double(c);
            push_double(c, (fmod(a, b)));
        } else {
            parser_func(token, c);
        }
    }
    double result = pop_double(c);
    return result;
}
