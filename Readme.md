Custom Shell 

A minimalist command-line shell written in **C**, built from scratch with raw terminal input handling,  command autocomplete using a **Trie**

 Features

- **Trie-based Autocomplete**
  - Suggests matching commands as you type.
  - Inline gray suggestion 
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
```sh
CUSTOM_SHELL/
├── include/ 
│ ├── raw_input.h
│ ├── readline_init.h
│ ├── shell.h
│ └── trie.h
│
├── src/ 
│ ├── autocomplete.c
│ ├── builtins.c 
│ ├── execute.c
│ ├── history.c 
│ ├── init.c 
│ ├── main.c 
│ ├── parse.c
│ ├── raw_input.c 
│ ├── readline_init.c 
│ └── trie.c 
│
├── .gitignore
├── Makefile
└── README.md

