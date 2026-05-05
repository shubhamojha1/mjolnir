#include <stdlib.h>
#include <string.h>
#include "history.h"

static char *history[HISTORY_MAX];
static int history_count = 0;

void history_init(void) {
    memset(history, 0, sizeof(history));
    history_count = 0;
}

void history_add(const char *line) {
    if (!line || !*line) return;

    if (history_count > 0 && strcmp(history[history_count - 1], line) == 0) return;

    if (history_count >= HISTORY_MAX) {
        free(history[0]);
        memmove(&history[0], &history[1], sizeof(char *) * (HISTORY_MAX - 1));
        history_count--;
    }

    history[history_count] = strdup(line);
    history_count++;
}

void history_free(void) {
    for (int i = 0; i < history_count; i++) {
        free(history[i]);
    }
    history_count = 0;
}
