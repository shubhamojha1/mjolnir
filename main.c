#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokenizer.h"
#include "history.h"
#include "builtins.h"

void print_welcome_art(void);
void shell_loop(void);

int main(int argc, char **argv){
    history_init();
    shell_loop();
    history_free();
    return EXIT_SUCCESS;
}

void print_welcome_art(void) {
    printf("⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n");
    printf("⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⣡⡾⠋⠙⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n");
    printf("⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢟⣵⡿⠋⠀⠀⠀⠀⠙⢿⣿⣿⣿⣿⣿⣿⣿⣿\n");
    printf("⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣱⡿⠋⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⣿⣿⣿⣿⣿⣿\n");
    printf("⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⣿⣿⣿⣿\n");
    printf("⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⣿⣿\n");
    printf("⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⡾⣿⣿\n");
    printf("⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⣧⡙⢿⣦⡀⠀⠀⠀⠀⠀⠀⣠⣾⢋⣴⣿⣿\n");
    printf("⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣯⣤⣬⣿⣶⣿⣿⣦⡀⠀⠀⣠⣾⢟⣵⣿⣿⣿⣿\n");
    printf("⣿⣿⣿⣿⣿⣿⣿⣿⣿⣯⣤⣤⣤⣾⣿⣿⣿⣿⣿⣿⣦⣾⣯⣵⣿⣿⣿⣿⣿⣿\n");
    printf("⣿⣿⣿⣿⣿⣿⠟⠉⠉⠉⢩⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n");
    printf("⣿⣿⣿⣿⡿⣿⡛⠛⢛⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n");
    printf("⣿⣿⡏⠙⢷⣌⣻⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n");
    printf("⣿⣿⣷⣄⣀⣹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n");
    printf("⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n");
}

void shell_loop(void){
    char *line = NULL;
    size_t linesize = 0;
    char **args;
    int status = 1;

    while (status) {
        printf("mjolnir> ");
        fflush(stdout);

        if (getline(&line, &linesize, stdin) == -1) {
            printf("\n");
            break;
        }

        if (line[strcspn(line, "\n")] != '\0') {
            line[strcspn(line, "\n")] = '\0';
        }

        if (!*line) continue;

        history_add(line);

        args = split_line(line);
        status = execute(args);

        free(args);
    }

    free(line);
}
