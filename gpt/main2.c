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

int find_pipe(char **argv) {
    int i;
    for (i = 0; argv[i] != NULL; i++) {
        if (strcmp(argv[i], "|") == 0)
            return i;
    }
    return -1;
}

void execute_command(char **argv) {
    if (execvp(*argv, argv) < 0) {
        printf("Command not found: %s\n", *argv);
        exit(1);
    }
}

void execute_pipe(char **argv, int pipe_position) {
    char *left_args[MAX_ARGS];
    char *right_args[MAX_ARGS];

    memcpy(left_args, argv, pipe_position * sizeof(char *));
    left_args[pipe_position] = NULL;
    memcpy(right_args, &argv[pipe_position + 1], (MAX_ARGS - pipe_position - 1) * sizeof(char *));
    right_args[MAX_ARGS - pipe_position - 1] = NULL;

    int pipefd[2];
    pid_t left_pid, right_pid;

    if (pipe(pipefd) < 0) {
        perror("Pipe error");
        exit(1);
    }

    left_pid = fork();
    if (left_pid < 0) {
        perror("Fork error");
        exit(1);
    }

    if (left_pid == 0) { // Left-side child process
        close(pipefd[0]); // Close read end of the pipe
        dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to the pipe
        close(pipefd[1]);
        execute_command(left_args);
    }

    right_pid = fork();
    if (right_pid < 0) {
        perror("Fork error");
        exit(1);
    }

    if (right_pid == 0) { // Right-side child process
        close(pipefd[1]); // Close write end of the pipe
        dup2(pipefd[0], STDIN_FILENO); // Redirect stdin to the pipe
        close(pipefd[0]);
        execute_command(right_args);
    }

    // Parent process
    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(left_pid, NULL, 0);
    waitpid(right_pid, NULL, 0);
}

int main() {
    char line[MAX_LINE]; // Input buffer
    char *argv[MAX_ARGS]; // Argument list
    pid_t pid; // Process ID
    int status;

    printf("Welcome to the simple shell with piping support!\n");
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
        int pipe_position = find_pipe(argv);

        // Fork a child process
        pid = fork();

        if (pid < 0) { // Fork error
            fprintf(stderr, "Fork error. Exiting...\n");
            exit(1);
        } else if (pid == 0) { // Child process
            if (pipe_position != -1) {
                execute_pipe(argv, pipe_position);
            } else {
                execute_command(argv);
            }
        } else { // Parent process
            // Wait for the child process to terminate
            while (wait(&status) != pid)
                ;
        }
    }

    return 0;
}
