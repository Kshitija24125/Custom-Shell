#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/trie.h"  

#define MAX_INPUT 1024
#define MAX_HISTORY 100

static char* history[MAX_HISTORY];
static int history_count = 0;
static int history_index = 0;

void add_history_entry(const char* line) {
    if (history_count < MAX_HISTORY) {
        history[history_count++] = strdup(line);
    } else {
        free(history[0]);
        memmove(&history[0], &history[1], sizeof(char*) * (MAX_HISTORY - 1));
        history[MAX_HISTORY - 1] = strdup(line);
    }
}





void set_raw_mode(int enable) {
    static struct termios old, raw;
    if (enable) {
        tcgetattr(STDIN_FILENO, &old);
        raw = old;
        raw.c_lflag &= ~(ICANON | ECHO); // disable canonical mode and echo
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    } else {
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &old);
    }
}

char* read_input_with_suggestions() {
    char buffer[MAX_INPUT] = {0};
    int pos = 0;
            char** matches = NULL;
            int match_count = 0;

    set_raw_mode(1);
    write(STDOUT_FILENO, "mysh> ", 6);

    while (1) {
        char c;
        if (read(STDIN_FILENO, &c, 1) != 1) break;
        
        
        if (c == '\n') {
            write(STDOUT_FILENO, "\n", 1);
            break;
        } else if (c == 127 || c == '\b') { 
            if (pos > 0) {
                pos--;
                buffer[pos] = '\0';
            }
        } else if (c == '\t') {
            if (match_count == 1 && matches && matches[0]) {
                const char *completion = matches[0] + strlen(buffer);
                while (*completion && pos < MAX_INPUT - 1) {
                    buffer[pos++] = *completion++;
                }
                buffer[pos] = '\0';
            }  else if (match_count > 1 && matches) {
                printf("\n");
                for (int i = 0; i < match_count; ++i) {
                    printf("  %s\n", matches[i]);
                }
            }
        } else if (isprint(c) && pos < MAX_INPUT - 1) {
            buffer[pos++] = c;
            buffer[pos] = '\0';
        }
      if (matches) {
            for (int i = 0; i < match_count; i++) free(matches[i]);
            free(matches);
            matches = NULL;
            match_count = 0;
        }

        findSuggestions(command_trie, buffer, &matches, &match_count);

        const char* suggestion = "";
        if (match_count > 0 && matches[0]) {
            int len = strlen(matches[0]);
            for (int i = 1; i < match_count; i++) {
                int j = 0;
                while (j < len && matches[i][j] == matches[0][j]) j++;
                len = j;
            }
            if ((int)strlen(buffer) < len)
                suggestion = matches[0] + strlen(buffer);  
        }

        printf("\r\033[Kmysh> %s", buffer);
        if (*suggestion) {
            printf("\033[38;2;150;150;150m%s\033[0m", suggestion);
        }
        fflush(stdout);

       
    }

    set_raw_mode(0);
    return strdup(buffer);
}
