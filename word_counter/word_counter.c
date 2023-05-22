#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage:s <input_file> <word_to_count>\n");
        exit(1);
    }

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(1);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) { // Child process
        close(pipefd[0]); // Close unused read end

        FILE *input_file = fopen(argv[1], "r");
        if (input_file == NULL) {
            perror("fopen");
            exit(1);
        }

        char buffer[BUFFER_SIZE];
        size_t bytes_read;
        int word_count = 0;
        char *word_to_count = argv[2];

        while ((bytes_read = fread(buffer, sizeof(char), BUFFER_SIZE, input_file)) > 0) {
            char *token = strtok(buffer, " \n\t");
            while (token != NULL) {
                if (strcmp(token, word_to_count) == 0) {
                    word_count++;
                }
                token = strtok(NULL, " \n\t");
            }
        }

        fclose(input_file);
        write(pipefd[1], &word_count, sizeof(word_count));
        close(pipefd[1]); // Close write end
        exit(0);
    } else { // Parent process
        close(pipefd[1]); // Close unused write end

        int word_count;
        ssize_t bytes_read = read(pipefd[0], &word_count, sizeof(word_count));
        if (bytes_read == -1) {
            perror("read");
            exit(1);
        }

        FILE *output_file = fopen("output.txt", "w");
        if (output_file == NULL) {
            perror("fopen");
            exit(1);
        }

        fprintf(output_file, "The word '%s' appears %d times.\n", argv[2], word_count);
        fclose(output_file);
        close(pipefd[0]); // Close read end

        wait(NULL); // Wait for child process to finish
    }

    return 0;
}
