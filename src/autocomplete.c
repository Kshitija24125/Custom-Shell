#include "../include/trie.h"
#include "../include/shell.h"

#include <readline/readline.h>
#include <readline/history.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>


// Initialize command trie with all supported commands
void initialize_command_trie() {
    if (command_trie) freeTrie(command_trie);
    command_trie = createNode();
    
    // Basic shell commands
    insert(command_trie, "cd", "Change the current directory");
    insert(command_trie, "echo", "Display a line of text");
    insert(command_trie, "pwd", "Print working directory");
    insert(command_trie, "exit", "Exit the shell");
    insert(command_trie, "history", "Display command history");
    
    // File operations
    insert(command_trie, "touch", "Create empty file(s)");
    insert(command_trie, "mkdir", "Create directory(ies)");
    insert(command_trie, "cat", "Concatenate and display files");
    insert(command_trie, "head", "Display first part of files");
    insert(command_trie, "chmod", "Change file permissions");
    insert(command_trie, "ls", "List directory contents");
    insert(command_trie, "clear", "Clear the terminal screen");
    insert(command_trie, "help", "Display help information");
    
    
}

// Command generator for trie-based completion
char *command_generator(const char *text, int state) {
    static char **matches = NULL;
    static int match_count = 0, current_match = 0;
    
    if (!state) {
        if (matches) {
            for (int i = 0; i < match_count; i++) free(matches[i]);
            free(matches);
        }
        matches = NULL;
        match_count = 0;
        current_match = 0;
        
        findSuggestions(command_trie, text, &matches, &match_count);
    }
    
    if (current_match < match_count) {
        return strdup(matches[current_match++]);
    }
    
    return NULL;
}

// File/directory generator for path completion
char *file_generator(const char *text, int state) {
    static DIR *dir = NULL;
    static size_t text_len;
    static struct dirent *ent;
    
    if (!state) {
        text_len = strlen(text);
        char *dir_path = strdup(text);
        char *last_slash = strrchr(dir_path, '/');
        
        if (last_slash) {
            *last_slash = '\0';
            dir = opendir(*dir_path ? dir_path : "/");
        } else {
            dir = opendir(".");
        }
        free(dir_path);
    }
    
    while ((ent = readdir(dir)) != NULL) {
        if (strncmp(ent->d_name, text, text_len) == 0) {
            struct stat st;
            char full_path[1024];
            
            snprintf(full_path, sizeof(full_path), "%s/%s", 
                    text[0] == '/' ? "" : ".", ent->d_name);
            
            if (stat(full_path, &st) == 0) {
                char *suggestion = malloc(strlen(ent->d_name) + 2);
                sprintf(suggestion, "%s%c", ent->d_name, 
                       S_ISDIR(st.st_mode) ? '/' : '\0');
                return suggestion;
            }
        }
    }
    
    if (dir) {
        closedir(dir);
        dir = NULL;
    }
    return NULL;
}

// Display matches with hierarchical formatting
void display_matches(char **matches, int num_matches, int max_len) {
    (void)max_len;
    
    if (!matches || num_matches < 2) return;

    // Check for color support
    int use_color = isatty(STDOUT_FILENO) && getenv("TERM") != NULL;
    
    for (int i = 1; i < num_matches; i++) {
        char *match = matches[i];
        
            int common = 0;
            while (rl_line_buffer[common] && match[common] && 
                   rl_line_buffer[common] == match[common]) {
                common++;
            }
            
        
         
            printf("\r\033[K");
        
            fwrite(match, sizeof(char), common, stdout);

            if (use_color) {
                // Print suggestion suffix in gray
                printf("\x1b[38;2;150;150;150m%s\x1b[0m", match + common);                    
                         } else {
                printf("%s\n", match + common);
            }
        }
    
    rl_forced_update_display();
}
// Main completion function
char **custom_completion(const char *text, int start, int end) {
    (void)end;
    rl_attempted_completion_over = 1;
    
    if (start == 0) {
        return rl_completion_matches(text, command_generator);
    }
    return rl_completion_matches(text, file_generator);
}

// Initialize autocomplete system
void init_autocomplete(void) {
    initialize_command_trie();
    rl_attempted_completion_function = custom_completion;
    rl_completion_display_matches_hook = display_matches;
    rl_completion_append_character = '\0';
    rl_basic_word_break_characters = " \t\n\"\\'`@$><=;|&{(";
    rl_completion_query_items = 50;  // Limit displayed matches
}