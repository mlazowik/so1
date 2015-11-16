#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include "vstring.h"
#include "stack.h"
#include "converter.h"

static char* trim_whitespace(char *str) {
    while (*str != '\0' && isspace(*str)) {
        str++;
    }

    return str;
}

char* converter_next_step(char *expr, char **stack, char **partial) {
    expr = trim_whitespace(expr);

    size_t len = 0;

    while (expr[len] != '\0' && !isspace(expr[len])) {
        len++;
    }

    char token[len + 1];

    for (size_t i = 0; i < len; i++) {
        token[i] = *expr++;
    }

    token[len] = '\0';

    if (strlen(token) == 1) {
        if (token[0] == '(') {
            stack_push(stack, token[0]);
        } else if (token[0] == ')') {
            stack_pop(stack);
        } else {
            vstring_append(partial, token);
            vstring_append(partial, " ");
        }
    }

    return expr;
}