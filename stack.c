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

char stack_pop(char **stack) {
    size_t len = strlen(*stack);

    assert(len > 0);

    char ret = *stack[len - 1];

    vstring_pop(stack);

    return ret;
}