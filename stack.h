#ifndef STACK_H
#define STACK_H

#include <stdbool.h>

/* Wstawia element na stos (realokujac pamiec) */
void stack_push(char **stack, char val);

/* Sprawdza czy stos jest pusty */
bool stack_empty(char **stack);

/* Zwraca wierzcholek stosu */
char stack_top(char **stack);

/* Usuwa wierzcholek stosu i zwraca jego wartosc */
char stack_pop(char **stack);

#endif