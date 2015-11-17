#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "vstring.h"

static void resize(char **str, size_t len) {
    char *new_str = realloc(*str, sizeof(char) * (len + 1));

    if (new_str == NULL) {
        free(*str);
        fprintf(stderr, "Error reallocating memory");
        exit(1);
    }

    *str = new_str;
}

void vstring_append(char **str, char *suf) {
    size_t len = strlen(*str) + strlen(suf);

    resize(str, len);

    strcat(*str, suf);
}

void vstring_pop(char **str) {
    size_t len = strlen(*str) - 1;

    resize(str, len);

    (*str)[len] = '\0';
}