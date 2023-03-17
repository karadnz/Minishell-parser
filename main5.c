#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE 1024
#define MAX_ARGS 64
#define MAX_COMMANDS 16
#define MAX_HEREDOC 4096

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

typedef enum {
    NONE,
    PIPE,
    REDIRECT_IN,
    REDIRECT_OUT,
    HEREDOC,
    CONDITIONAL_AND,
    CONDITIONAL_OR
} CommandType;

typedef struct {
    CommandType type;
    int index;
    char *delimiter;
} CommandInfo;

CommandInfo find_command_type(char **argv) {
    CommandInfo info = {NONE, -1, NULL};

    for (int i = 0; argv[i] != NULL; i++) {
        if (strcmp(argv[i], "|") == 0) {
            info.type = PIPE;
            info.index = i;
            break;
        } else if (strcmp(argv[i], "<") == 0) {
            info.type = REDIRECT_IN;
            info.index = i;
        } else if (strcmp(argv[i], "<<") == 0) {
            info.type = HEREDOC;
            info.index = i;
            info.delimiter = argv[i + 1];
        } else if (strcmp(argv[i], ">") == 0) {
            info.type = REDIRECT_OUT;
            info.index = i;
        } else if (strcmp(argv[i], "&&") == 0) {
            info.type = CONDITIONAL_AND;
            info.index = i;
        } else if (strcmp(argv[i], "||") == 0) {
            info.type = CONDITIONAL_OR;
            info.index = i;
        }
    }

    return info;
}

void execute_command(char **argv) {
    if (execvp(*argv, argv) < 0) {
        printf("Command not found: %s\n", *argv);
        exit(1);
    }
}

void execute_heredoc(char *delimiter) {
    int pipefd[2];
    pipe(pipefd);

    if (fork() == 0) {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        char buffer[MAX_HEREDOC];
        while (1) {
            printf("> ");
            fgets(buffer, MAX_HEREDOC, stdin);

            if (strncmp(buffer, delimiter, strlen(delimiter)) == 0 &&
                (buffer[strlen(delimiter)] == '\n' || buffer[strlen(delimiter)] == '\0'))
                break;

            write(STDOUT_FILENO, buffer, strlen(buffer));
        }

        exit(0);
    } else {
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);

        wait(NULL);
    }
}

void handle_command(CommandInfo info, char **argv) {
    if (info.type == REDIRECT_IN || info.type == REDIRECT_OUT || info.type == HEREDOC) {
        int fd;

        if (info.type == REDIRECT_IN) {
            fd = open(argv[info.index + 1], O_RDONLY);
            if (fd < 0) {
                perror("open");
                exit(1);
            }
            dup2(fd, STDIN_FILENO);
        } else if (info.type == REDIRECT_OUT) {
            fd = open(argv[info.index + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                perror("open");
                exit(1);
            }
            dup2(fd, STDOUT_FILENO);
        } else if (info.type == HEREDOC) {
            execute_heredoc(info.delimiter);
        }

        argv[info.index] = NULL;
    }

    if (info.type != HEREDOC) {
        execute_command(argv);
    } else {
        execute_command(argv + info.index + 2);
    }

    exit(0);
}

int main() {
    char line[MAX_LINE]; // Input buffer
    char *argv[MAX_ARGS]; // Argument list

    printf("Welcome to the simple shell with multiple piping, redirection, heredocs, and conditional execution support!\n");
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

        // Find command type and index
        CommandInfo info = find_command_type(argv);

        int status;
        pid_t pid = fork();

        if (pid < 0) { // Fork error
            fprintf(stderr, "Fork error. Exiting...\n");
            exit(1);
        } else if (pid == 0) { // Child process
            handle_command(info, argv);
        } else { // Parent process
            // Wait for the child process to terminate
            wait(&status);

            if (info.type == CONDITIONAL_AND && status != 0) {
                break;
            } else if (info.type == CONDITIONAL_OR && status == 0) {
                break;
            }
        }
    }

    return 0;
}