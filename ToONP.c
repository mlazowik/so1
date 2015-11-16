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

    char *str = malloc(sizeof(char) * (len + 1));

    if ((read_len = read(read_dsc, str, sizeof(char) * (len + 1))) == -1) {
        syserr("Error in read\n");
    }

    return str;
}

void step(char *expr, char *stack, char *partial) {
    fprintf(stderr, "expr = %s\t", expr);
    fprintf(stderr, "stack = %s\t", stack);
    fprintf(stderr, "partial = %s\n", partial);

    if (*expr == '\0') {
        return;
    }

    pid_t pid;

    int send_dsc[2], rcv_dsc[2];

    if (pipe(send_dsc) == -1 || pipe(rcv_dsc) == -1) {
        syserr("Error in pipe\n");
    }

    switch(pid = fork()) {
        case -1:
            syserr("Error in fork\n");

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

            char *next_expr = converter_next_step(expr, &stack, &partial);

            step(next_expr, stack, partial);

            free(expr);
            free(stack);
            free(partial);

            exit(0);

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

            if (wait(0) == -1) {
                syserr("Error in wait");
            }

            exit(0);
    }
}

int main(int argc, char *argv[]) {
    assert(argc == 2);

    char *stack = malloc(sizeof(char));
    stack[0] = '\0';

    char *partial = malloc(sizeof(char));
    partial[0] = '\0';

    step(argv[1], stack, partial);

    return 0;
}