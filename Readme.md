## Custom Shell 

CUSTOM_SHELL/
├── include/ # Header files
│ ├── raw_input.h
│ ├── readline_init.h
│ ├── shell.h
│ └── trie.h
│
├── src/ # C source files
│ ├── autocomplete.c # Completion logic
│ ├── builtins.c # Built-in commands
│ ├── execute.c # Command execution
│ ├── history.c # Command history logic
│ ├── init.c # Shell init
│ ├── main.c # Shell entry point
│ ├── parse.c # Argument parsing
│ ├── raw_input.c # Raw input handling (termios)
│ ├── readline_init.c # Autocomplete init
│ └── trie.c # Trie implementation
│
├── .gitignore
├── Makefile
└── README.md
A minimalist command-line shell written in **C**, built from scratch with raw terminal input handling, smart command autocomplete using a **Trie**

##  Features

- **Trie-based Autocomplete**
  - Suggests matching commands as you type.
  - Inline gray suggestion (like Zsh or Fish).
  - Press `Tab` to auto-complete when only one match exists.

- **Raw Terminal Input**
  - Fully manual input reading using `termios`.
  - Supports `Backspace`, `Tab`.

-  **Command History**
  - Easy to extend for persistent history storage.

-  **Built-in Commands**
    cd, pwd, echo, exit, history
    touch, mkdir, cat, head, tail
    chmod, ls, clear, help

##  Project Structure



