#ifndef STACK_H
#define STACK_H

#include <stdbool.h>

void stack_push(char **stack, char val);

bool stack_empty(char **stack);

char stack_top(char **stack);

char stack_pop(char **stack);

#endif