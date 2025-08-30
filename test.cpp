#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <editline/readline.h>

// Our list of commands for tab completion
const char *commands[] = {
    "help",
    "exit",
    "list",
    "show",
    "run",
    "history",
    NULL
};

// Generator function for tab completion
char *command_generator(const char *text, int state) {
    static int list_index, len;
    if (!state) {
        list_index = 0;
        len = strlen(text);
    }

    const char *name;
    while ((name = commands[list_index++])) {
        if (strncmp(name, text, len) == 0) {
            return strdup(name);  // return match
        }
    }
    return NULL;
}

// Tell readline to use our completion function
char **custom_completion(const char *text, int start, int end) {
    (void)start; // unused
    (void)end;   // unused
    return rl_completion_matches(text, command_generator);
}

int main() {
    rl_attempted_completion_function = custom_completion;

    char *input;
    while (1) {
        input = readline("myshell> ");

        if (!input) {
            printf("\nExit\n");
            break;  // Ctrl+D
        }

        if (*input) {
            add_history(input);  // allows Up/Down arrows
        }

        if (strcmp(input, "exit") == 0) {
            free(input);
            break;
        }

        printf("You typed: %s\n", input);
        free(input);
    }
    return 0;
}