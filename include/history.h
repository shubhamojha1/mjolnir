#ifndef HISTORY_H
#define HISTORY_H

#define HISTORY_MAX 50

void history_init(void);
void history_add(const char *line);
void history_free(void);

#endif
