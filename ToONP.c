#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>
#include <string.h>
#include "err.h"
#include "converter.h"

void write_string(int write_dsc, char *str) {
    size_t len = strlen(str);

    if (write(write_dsc, &len, sizeof(size_t)) == -1) {
        syserr("Error in write\n");
    }

    if (write(write_dsc, str, sizeof(char) * (len + 1)) == -1) {
        syserr("Error in write\n");
    }
}

char* read_string(int read_dsc) {
    size_t len, read_len;

    if ((read_len = read(read_dsc, &len, sizeof(size_t))) == -1) {
        syserr("Error in read\n");
    }

    assert(read_len == sizeof(size_t));

    char *str = emalloc(sizeof(char) * (len + 1));

    if ((read_len = read(read_dsc, str, sizeof(char) * (len + 1))) == -1) {
        syserr("Error in read\n");
    }

    return str;
}

/* tworzy proces odpowiedzialny za kolejny krok algorytmu */
void step(char *expr, char *stack, char *partial, int prev_pipe) {
    pid_t pid;

    int send_dsc[2], rcv_dsc[2];

    if (pipe(send_dsc) == -1 || pipe(rcv_dsc) == -1) {
        syserr("Error in pipe\n");
    }

    switch(pid = fork()) {
        case -1:
            syserr("Error in fork\n");

        // potomek
        case 0:
            if (close(send_dsc[1]) == -1) {
                syserr("Error in close send_dsc[1]\n");
            }

            if (close(rcv_dsc[0]) == -1) {
                syserr("Error in close rcv_dsc[0]\n");
            }

            expr = read_string(send_dsc[0]);
            stack = read_string(send_dsc[0]);
            partial = read_string(send_dsc[0]);

            if (close(send_dsc[0]) == -1) {
                syserr("Error in close send_dsc[0]\n");
            }

            char *next_expr = converter_next_step(expr, &stack, &partial);

            if (*next_expr != '\0') {
                step(next_expr, stack, partial, rcv_dsc[1]);
            } else {
                write_string(rcv_dsc[1], partial);

                if (close(rcv_dsc[1]) == -1) {
                    syserr("Error in close rcv_dsc[1]\n");
                }
            }

            free(expr);
            free(stack);
            free(partial);

            return;

        // rodzic
        default:
            if (close(send_dsc[0]) == -1) {
                syserr("Error in close send_dsc[0]\n");
            }

            if (close(rcv_dsc[1]) == -1) {
                syserr("Error in close rcv_dsc[1]\n");
            }

            write_string(send_dsc[1], expr);
            write_string(send_dsc[1], stack);
            write_string(send_dsc[1], partial);

            if (close(send_dsc[1]) == -1) {
                syserr("Error in close send_dsc[1]\n");
            }

            if (wait(0) == -1) {
                syserr("Error in wait");
            }

            char *res = read_string(rcv_dsc[0]);

            // prev_pipe - lacze zwrotne do poprzedniego procesu
            if (prev_pipe != -1) {
                write_string(prev_pipe, res);
                if (close(prev_pipe) == -1) {
                    syserr("Error in close prev_pipe\n");
                }
            } else {
                // jesli nie istnieje => jest to pierwszy proces => wypisz wynik
                fprintf(stderr, "%s\n", res);
            }

            free(res);

            exit(0);
    }
}

int main(int argc, char *argv[]) {
    assert(argc == 2);

    char *stack = emalloc(sizeof(char));
    stack[0] = '\0';

    char *partial = emalloc(sizeof(char));
    partial[0] = '\0';

    step(argv[1], stack, partial, -1);

    return 0;
}
