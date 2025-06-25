#include "../include/shell.h"
#include<stdbool.h>

// Helper function to count pipelines
 int count_pipes(char **args) {
    int count = 0;
    for (int i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], "|") == 0) {
            count++;
        }
    }
    return count;
}

// Main parsing function
int parse_input(char *input, char **args) {
    int arg_count = 0;
    char *token = strtok(input, " \t\n\r");
    
    while (token != NULL && arg_count < MAX_ARGS-1) {
        args[arg_count++] = token;
        token = strtok(NULL, " \t\n\r");
    }
    args[arg_count] = NULL;
    
    return arg_count;
}

// Parse pipeline commands into separate command arrays
int parse_pipeline(char **args, char ***commands) {
    int cmd_count = 0;
    int start = 0;
    
    for (int i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], "|") == 0) {
            args[i] = NULL; // Terminate current command
            commands[cmd_count++] = &args[start];
            start = i + 1;
            
            if (cmd_count >= MAX_CMDS-1) {
                fprintf(stderr, "Error: Too many pipes\n");
                return -1;
            }
        }
    }
    
    // Add the last command
    commands[cmd_count++] = &args[start];
    commands[cmd_count] = NULL;
    
    return cmd_count;
}

// Parse redirection symbols (called before execution)
void parse_redirection(char **args, int *in_fd, int *out_fd) {
    *in_fd = -1;
    *out_fd = -1;
    
    for (int i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], "<") == 0) {
            if (args[i+1] == NULL) {
                fprintf(stderr, "Syntax error: no input file\n");
                continue;
            }
            *in_fd = open(args[i+1], O_RDONLY);
            if (*in_fd == -1) {
                perror("open");
            }
            args[i] = NULL; // Remove redirection from args
        }
        else if (strcmp(args[i], ">") == 0) {
            if (args[i+1] == NULL) {
                fprintf(stderr, "Syntax error: no output file\n");
                continue;
            }
            *out_fd = open(args[i+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (*out_fd == -1) {
                perror("open");
            }
            args[i] = NULL;
        }
        else if (strcmp(args[i], ">>") == 0) {
            if (args[i+1] == NULL) {
                fprintf(stderr, "Syntax error: no output file\n");
                continue;
            }
            *out_fd = open(args[i+1], O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (*out_fd == -1) {
                perror("open");
            }
            args[i] = NULL;
        }
    }
}

// Check if input contains pipeline
bool has_pipeline(char **args) {
    for (int i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], "|") == 0) {
            return true;
        }
    }
    return false;
}