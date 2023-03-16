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

int main() {
    char line[MAX_LINE]; // Input buffer
    char *argv[MAX_ARGS]; // Argument list
    pid_t pid; // Process ID
    int status;

    printf("Welcome to the simple shell!\n");
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

        // Fork a child process
        pid = fork();

        if (pid < 0) { // Fork error
            fprintf(stderr, "Fork error. Exiting...\n");
            exit(1);
        } else if (pid == 0) { // Child process
            if (execvp(*argv, argv) < 0) {
                printf("Command not found: %s\n", *argv);
                exit(1);
            }
        } else { // Parent process
            // Wait for the child process to terminate
            while (wait(&status) != pid)
                ;
        }
    }

    return 0;
}
