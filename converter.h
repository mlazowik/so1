#ifndef CONVERTER_H
#define CONVERTER_H

char* converter_next_step(char *expr, char **stack, char **partial);

void converter_pop_remaining_operators(char **stack, char **partial);

#endif