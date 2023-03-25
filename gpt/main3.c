#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE 1024
#define MAX_ARGS 64

// Function to parse input into an array of arguments
void parse_input(char *line, char **argv) {
    while (*line != '\0') {
        // Replace whitespaces with null terminators
        while (*line == ' ' || *line == '\t' || *line == '\n')
            *line++ = '\0';

        // Save the argument position
        *argv++ = line;

        // Skip to the next whitespace
        while (*line != '\0' && *line != ' ' && *line != '\t' && *line != '\n')
            line++;
    }

    // Mark the end of the argument list
    *argv = '\0';
}

int count_pipes(char **argv) {
    int count = 0;
    for (int i = 0; argv[i] != NULL; i++) {
        if (strcmp(argv[i], "|") == 0)
            count++;
    }
    return count;
}

void extract_commands(char **argv, int pipes_count, char ***commands) {
    int cmd_index = 0;
    commands[cmd_index++] = argv;

    for (int i = 0; argv[i] != NULL; i++) {
        if (strcmp(argv[i], "|") == 0) {
            argv[i] = NULL;
            commands[cmd_index++] = &argv[i + 1];
        }
    }
    commands[cmd_index] = NULL;
}

void execute_command(char **argv) {
    if (execvp(*argv, argv) < 0) {
        printf("Command not found: %s\n", *argv);
        exit(1);
    }
}

void execute_pipes(char **argv) {
    int pipes_count = count_pipes(argv);
    char **commands[pipes_count + 2];
    extract_commands(argv, pipes_count, commands);

    int pipefds[2 * pipes_count];

    for (int i = 0; i < pipes_count; i++) {
        if (pipe(pipefds + i * 2) < 0) {
            perror("Pipe error");
            exit(1);
        }
    }

    int pid;
    for (int i = 0; i <= pipes_count; i++) {
        pid = fork();
        if (pid < 0) {
            perror("Fork error");
            exit(1);
        }

        if (pid == 0) { // Child process
            if (i != pipes_count) {
                dup2(pipefds[i * 2 + 1], STDOUT_FILENO);
            }

            if (i != 0) {
                dup2(pipefds[(i - 1) * 2], STDIN_FILENO);
            }

            for (int j = 0; j < 2 * pipes_count; j++) {
                close(pipefds[j]);
            }

            execute_command(commands[i]);
        }
    }

    // Parent process
    for (int j = 0; j < 2 * pipes_count; j++) {
        close(pipefds[j]);
    }

    for (int j = 0; j <= pipes_count; j++) {
        wait(NULL);
    }
}

int main() {
    char line[MAX_LINE]; // Input buffer
    char *argv[MAX_ARGS]; // Argument list

    printf("Welcome to the simple shell with multiple piping support!\n");
    printf("Type 'exit' to quit.\n");

    while (1) {
        // Prompt user for input
        printf("> ");
        fgets(line, MAX_LINE, stdin);

        // Remove trailing newline character
        line[strcspn(line, "\n")] = '\0';

        // Check if the user entered "exit"
        if (strcmp(line, "exit") == 0)
            exit(0);

        // Parse input
        parse_input(line, argv);

        // Check for pipes
        int pipes_count = count_pipes(argv);

        if (pipes_count > 0) {
            execute_pipes(argv);
        } else {
            // Fork a child process
            pid_t pid = fork();

            if (pid < 0) { // Fork error
                fprintf(stderr, "Fork error. Exiting...\n");
                exit(1);
            } else if (pid == 0) { // Child process
                execute_command(argv);
            } else { // Parent process
                // Wait for the child process to terminate
                wait(NULL);
            }
        }
    }

    return 0;
}
