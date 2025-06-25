#include "../include/shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_HISTORY 100

static char *history[MAX_HISTORY];
static int history_count = 0;

void init_history(void) {}

void add_to_history(const char *cmd) {
    if (history_count < MAX_HISTORY) {
        history[history_count++] = strdup(cmd);
    } else {
        // Rotate history when full
        free(history[0]);
        memmove(history, history+1, (MAX_HISTORY-1)*sizeof(char*));
        history[MAX_HISTORY-1] = strdup(cmd);
    }
}

int show_history(char **args) {
    int count = history_count;
    if (args[1]) count = atoi(args[1]);  // Allow: history [N]
    
    for (int i = (history_count > count) ? history_count - count : 0; 
         i < history_count; i++) {
        printf("%5d  %s\n", i+1, history[i]);
    }
    return 1;  // Return status for builtins
}
void free_history(void) {
    for (int i = 0; i < history_count; i++) {
        free(history[i]);
    }
    history_count = 0;
}