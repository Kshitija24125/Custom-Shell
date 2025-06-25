#include "../include/shell.h"

static void setup_redirection(int in_fd, int out_fd) {
    if (in_fd != -1) {
        dup2(in_fd, STDIN_FILENO);
        close(in_fd);
    }
    if (out_fd != -1) {
        dup2(out_fd, STDOUT_FILENO);
        close(out_fd);
    }
}

int execute_single_command(char **args) {
    int in_fd = -1, out_fd = -1;
    
    // Parse and handle redirections
    parse_redirection(args, &in_fd, &out_fd);
    
    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        setup_redirection(in_fd, out_fd);
        execvp(args[0], args);
        perror("execvp");
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("fork");
        return -1;
    }
    
    // Parent process - wait for completion
    int status;
    waitpid(pid, &status, 0);
    
    // Close any remaining file descriptors
    if (in_fd != -1) close(in_fd);
    if (out_fd != -1) close(out_fd);
    
    return WEXITSTATUS(status);
}

int execute_pipeline(char ***commands,int cmd_count) {
    int num_commands = 0;
    while (commands[num_commands] != NULL) {
        num_commands++;
    }

    int pipes[cmd_count - 1][2];
    pid_t pids[cmd_count];
    int status = 0;

    // Create pipes
    for (int i = 0; i < cmd_count - 1; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            return -1;
        }
    }


    // Fork processes
    for (int i = 0; i < num_commands; i++) {
        pids[i] = fork();
        if (pids[i] == 0) {
            // Child process
            
            // Input redirection (except first command)
            if (i > 0) {
                dup2(pipes[i-1][0], STDIN_FILENO);
            }

            // Output redirection (except last command)
            if (i < num_commands - 1) {
                dup2(pipes[i][1], STDOUT_FILENO);
            }

            // Close all pipe ends
            for (int j = 0; j < num_commands - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            // Handle file redirections within the command
            int in_fd = -1, out_fd = -1;
            parse_redirection(commands[i], &in_fd, &out_fd);
            setup_redirection(in_fd, out_fd);

            execvp(commands[i][0], commands[i]);
            perror("execvp");
            exit(EXIT_FAILURE);
        } else if (pids[i] < 0) {
            perror("fork");
            return -1;
        }
    }

    // Close all pipes in parent
    for (int i = 0; i < num_commands - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // Wait for all children
    for (int i = 0; i < num_commands; i++) {
        waitpid(pids[i], &status, 0);
        if (i == num_commands - 1) {
            status = WEXITSTATUS(status);
        }
    }

    return status;
}

int execute_command(char **args) {
    if (args[0] == NULL) {
        return 1;  // Empty command
    }

    // Handle built-ins (they don't support pipes/redirection)
    if (!has_pipeline(args) && handle_builtins(args)) {
        return 0;
    }

    // Check for pipeline
    if (has_pipeline(args)) {
        char *commands[MAX_CMDS][MAX_ARGS];
        int cmd_count = parse_pipeline(args, (char ***)commands);
        if (cmd_count <= 0) {
            fprintf(stderr, "Invalid pipeline\n");
            return -1;
        }
        return execute_pipeline((char ***)commands ,cmd_count);
    }

    // Regular command execution
    return execute_single_command(args);
}