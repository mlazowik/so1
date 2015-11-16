#ifndef VSTRING_H
#define VSTRING_H

/* dopisuje suf na koniec str */
void vstring_append(char **str, char *suf);

/* usuwa ostatni znak */
void vstring_pop(char **str);

#endif