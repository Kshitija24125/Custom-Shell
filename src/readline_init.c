#include <readline/readline.h>
#include <readline/history.h>
#include "../include/shell.h"
#include "../include/trie.h"  

void init_readline(void) {
    // Enable basic readline features
    initialize_command_trie();
    rl_readline_name = "mysh";
    
    // Enable colors
    if (isatty(STDOUT_FILENO) && getenv("TERM")) {
        rl_variable_bind("colored-completion-prefix", "1");
        rl_variable_bind("colored-stats", "1");
    }
    
    
    rl_completion_display_matches_hook = display_matches;
    // Configure completion
    rl_basic_word_break_characters = " \t\n\"\\'`@$><=;|&{(";
    rl_completion_append_character = '\0';
    rl_completion_query_items = 100;
    
    // Initialize your custom completions
    initialize_command_trie();
}