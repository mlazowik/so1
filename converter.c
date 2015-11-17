#include <ctype.h>
#include <string.h>
#include "vstring.h"
#include "stack.h"
#include "converter.h"

static int get_precedence(char op) {
    switch (op) {
        case '^': return 4;
        case '*': return 3;
        case '/': return 3;
        case '+': return 2;
        case '-': return 2;

        default: return -1;
    }
}

static char* trim_whitespace(char *str) {
    while (*str != '\0' && isspace(*str)) {
        str++;
    }

    return str;
}

static bool is_operator(char *token) {
    if (strlen(token) != 1) return false;

    return (get_precedence(token[0]) != -1);
}

char* converter_next_step(char *expr, char **stack, char **partial) {
    expr = trim_whitespace(expr);

    size_t len = 0;

    while (expr[len] != '\0' && !isspace(expr[len])) {
        len++;
    }

    char token[len + 1];

    for (size_t i = 0; i < len; i++) {
        token[i] = *expr;
        expr++;
    }

    token[len] = '\0';

    if (is_operator(token)) {
        int precedence = get_precedence(token[0]);

        while (!stack_empty(stack)
               && precedence <= get_precedence(stack_top(stack))) {
            char op[2];

            op[0] = stack_pop(stack);
            op[1] = '\0';

            vstring_append(partial, op);
            vstring_append(partial, " ");
        }

        stack_push(stack, token[0]);
    } else if (token[0] == '(') {
        stack_push(stack, '(');
    } else if (token[0] == ')') {
        while (stack_top(stack) != '(') {
            char op[2];

            op[0] = stack_pop(stack);
            op[1] = '\0';

            vstring_append(partial, op);
            vstring_append(partial, " ");
        }

        stack_pop(stack);
    } else {
        vstring_append(partial, token);
        vstring_append(partial, " ");
    }

    expr = trim_whitespace(expr);

    if (*expr == '\0') {
        while (!stack_empty(stack)) {
            char op[2];

            op[0] = stack_pop(stack);
            op[1] = '\0';

            vstring_append(partial, op);
            vstring_append(partial, " ");
        }
    }

    return expr;
}
