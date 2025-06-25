#ifndef SHELL_H
#define SHELL_H
#define _FUNCTION_DEF
#include<stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/stat.h>
#include <errno.h>

#if RL_VERSION_MAJOR >= 8
#define USE_RL_VARIABLES 1
#else
extern int rl_color;
extern int rl_colorize_completion;
extern char *rl_bell_style;
extern const char *rl_basic_word_break_characters;
extern int rl_completion_append_character;
extern int rl_completion_query_items;
#endif

#include "trie.h"

#define MAX_ARGS 64
#define MAX_CMDS 10


void init_readline(void);

// Function prototypes
void print_prompt(void);
int parse_input(char *input, char **args);
int execute_command(char **args);
int handle_builtins(char **args);
int execute_pipeline(char ***commands, int cmd_count);
void init_shell(void);
void cleanup_shell(void);

int parse_pipeline(char **args, char ***commands);
void parse_redirection(char **args, int *in_fd, int *out_fd);
bool has_pipeline(char **args);
int count_pipes(char **args);

// History entry structure
struct hist_entry {
    char *command;
    time_t timestamp;
};

void init_history(void);
void add_to_history(const char *cmd);
int show_history(char **args);
void free_history(void);
// Built-in commands
int shell_cd(char **args);
int shell_pwd(char **args);
int shell_exit(char **args);
int shell_help(char **args);

int shell_touch(char **args);
int shell_echo(char **args);
int shell_mkdir(char **args);
int shell_cat(char **args);
int shell_head(char **args);
int shell_chmod(char **args);

// Autocompletion
void init_autocomplete(void);
char **custom_completion(const char *text, int start, int end);
char *command_generator(const char *text, int state);
char *file_generator(const char *text, int state);

typedef struct TrieNode TrieNode;
TrieNode* createNode();
void insert(TrieNode* root, const char* path, const char* desc);
void freeTrie(TrieNode* root);
void initialize_command_trie();
void findSuggestions(TrieNode* root, const char* prefix, char*** matches, int* match_count);
void display_matches(char **matches, int num_matches, int max_len) ;


#endif

