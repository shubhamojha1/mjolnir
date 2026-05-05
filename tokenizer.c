#include <stdlib.h>
#include <stdio.h>
#include "tokenizer.h"

char **split_line(char *line){
    int bufsize = TOKEN_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));

    if(!tokens) {
        fprintf(stderr, "mjolnir: allocation error\n");
        exit(EXIT_FAILURE);
    }

    while(*line) {
        while(*line == ' ' || *line == '\t') line++;
        if (!*line) break;

        if (position >= bufsize) {
            bufsize += TOKEN_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if(!tokens) {
                fprintf(stderr, "mjolnir: allocation error!");
                exit(EXIT_FAILURE);
            }
        }

        tokens[position] = line;
        int in_quote = 0;
        int escape = 0;

        while(*line) {
            if (escape) {
                escape = 0;
                line++;
                continue;
            }

            if (*line == '\\' && !in_quote) {
                escape = 1;
                line++;
                continue;
            }

            if (*line == '"') {
                in_quote = !in_quote;
                line++;
                continue;
            }

            if (*line == '\'' && !in_quote) {
                in_quote = 1;
                line++;
                char *start = line;
                while(*line && *line != '\'') line++;
                if (*line == '\'') *line = '\0';
                line++;
                tokens[position] = start;
                position++;

                while(*line == ' ' || *line == '\t') line++;
                break;
            }

            if (!in_quote && (*line == ' ' || *line == '\t')) {
                *line = '\0';
                line++;
                position++;
                break;
            }

            line++;
        }

        if (!*line && tokens[position] != line) {
            position++;
        }
    }

    tokens[position] = NULL;
    return tokens;
}
