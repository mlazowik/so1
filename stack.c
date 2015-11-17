#include <assert.h>
#include <string.h>
#include "stack.h"
#include "vstring.h"

void stack_push(char **stack, char val) {
    char tmp[2];

    tmp[0] = val;
    tmp[1] = '\0';

    vstring_append(stack, tmp);
}

bool stack_empty(char **stack) {
    return (*stack)[0] == '\0';
}

char stack_top(char **stack) {
    assert(!stack_empty(stack));

    size_t len = strlen(*stack);

    return (*stack)[len - 1];
}

char stack_pop(char **stack) {
    assert(!stack_empty(stack));

    size_t len = strlen(*stack);

    char ret = (*stack)[len - 1];

    vstring_pop(stack);

    return ret;
}