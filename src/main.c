#include "../include/shell.h"
#include "../include/raw_input.h"  
#include "../include/trie.h"

int main() {
    char *args[MAX_ARGS];
    
    if (!isatty(STDOUT_FILENO)) {
        fprintf(stderr, "Warning: Not running in a terminal\n");    }
    else if (!getenv("TERM")) {
        fprintf(stderr, "Warning: TERM environment variable not set\n");
    }
    initialize_command_trie();  
    init_readline();
    init_autocomplete(); 
    init_shell();
    init_history();  


    while (1) {
        char* input = read_input_with_suggestions();  
        if (!input) break;
        
        if (strlen(input) > 0) {
            add_to_history(input);
            if (parse_input(input, args) > 0) {
                if (strcmp(args[0], "exit") == 0) {
                    free(input);
                    break;
                }
                execute_command(args);
            }
        }
        free(input);
    }
    free_history();  
    cleanup_shell();
    return 0;
}