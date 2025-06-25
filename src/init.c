#include "../include/shell.h"
#include <readline/history.h>
#include "../include/readline_init.h"

void init_shell() {

    initialize_command_trie();  
    rl_readline_name = "mysh";
    rl_attempted_completion_function = custom_completion;
    rl_variable_bind("colored-completion-prefix", "1");
    rl_variable_bind("colored-stats", "1");
    rl_variable_bind("bell-style", "none");
    rl_variable_bind("completion-ignore-case", "0");
    rl_basic_word_break_characters = " \t\n\"\\'`@$><=;|&{(";
}

void cleanup_shell() {}