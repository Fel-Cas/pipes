#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>
#include <getopt.h>
#include <locale.h>
#include <string.h>
#include "matrix.c"

void get_params(int argc, char *argv[], int *c, int *f, char *p) {
    int opt;
    setlocale(LC_ALL, "");
    while ((opt = getopt(argc, argv, "f:c:p:")) != -1) {
        switch (opt) {
            case 'c':
                *c = atoi(optarg);
                break;
            case 'f':
                *f = atoi(optarg);
                break;
            case 'p':
                strncpy(p, optarg, 200);
                p[199] = '\0'; // Ensure null-termination of the string
                break;
            default:
                fprintf(stderr, "Usage: %s -o <operation> [-f <value>] [-c <value>] [-e <value>] [-r <value>] [-s <value>] [-p <value>]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }
}

int main(int argc, char *argv[]) {
    int f = 0, c = 0;
    char p[200] = "";
    get_params(argc, argv, &f, &c, p);

    Matrix* matrix = create_matrix_from_file(p, f, c);

    int fd[2];
    if (pipe(fd) == -1) {
        perror("pipe");
        exit(1);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    } 
    if (pid == 0) {
    dup2(fd[0], STDIN_FILENO);
    close(fd[0]);

    // Read the matrix from STDIN_FILENO
    read(STDIN_FILENO, matrix, sizeof(Matrix));

    printf("Original matrix:\n");
    print_matrix(matrix);


    Vector* min = matrix_col_min(matrix);
    Vector* max = matrix_col_max(matrix);

    // Write the vectors' elements to STDOUT_FILENO
    for (int i = 0; i < max->size; i++) {
        write(fd[1], &(max->elements[i]), sizeof(double));
    }

    for (int i = 0; i < min->size; i++) {
        write(fd[1], &(min->elements[i]), sizeof(double));
    }
    close(fd[1]);
    exit(0);
} else {
    // Write the matrix to the pipe
    write(fd[1], matrix, sizeof(Matrix));
    close(fd[1]); // Close the write descriptor before waiting for the child process
    wait(NULL);

    Vector* max = create_vector(matrix->cols);
    Vector* min = create_vector(matrix->cols);

    // Read the vector elements from STDOUT_FILENO
    for (int i = 0; i < max->size; i++) {
        read(fd[0], &(max->elements[i]), sizeof(double));
    }

    for (int i = 0; i < min->size; i++) {
        read(fd[0], &(min->elements[i]), sizeof(double));
    }
    close(fd[0]);

    printf("Vector de números maximos:\n");
    print_vector(max);
    printf("Vector de números minimos:\n");
    print_vector(min);

    normalize_matrix_column_formula_1(matrix, max, min);

    printf("Matriz normalizada:\n");
    print_matrix(matrix);
}
    return 0;
}
