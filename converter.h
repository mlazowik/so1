#ifndef CONVERTER_H
#define CONVERTER_H

/* Wykonuje jeden etap konwersji do ONP. Jesli nie ma juz znacznikow do
przetworzenia wykonuje ostatni etap (zrzucenie stosu na wyjscie) */
char* converter_next_step(char *expr, char **stack, char **partial);

#endif
