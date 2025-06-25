SRC = src/main.c src/parse.c src/execute.c src/builtins.c  src/init.c src/history.c src/autocomplete.c   src/readline_init.c src/trie.c src/raw_input.c

OBJ = $(SRC:.c=.o)

all: mysh

mysh: $(OBJ)
	gcc -Wall -Wextra -I./include -o $@ $^ -lreadline

%.o: %.c
	gcc -Wall -Wextra -I./include -c -o $@ $<

clean:
	rm -f $(OBJ) mysh

.PHONY: all clean