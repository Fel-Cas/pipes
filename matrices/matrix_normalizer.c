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

    int parent_to_child[2];
    int child_to_parent[2];
    
    if (pipe(parent_to_child) == -1 || pipe(child_to_parent) == -1) {
        perror("pipe");
        exit(1);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    } 
    if (pid == 0) {
        close(parent_to_child[1]);  // Close the write end of the parent-to-child pipe
        close(child_to_parent[0]);  // Close the read end of the child-to-parent pipe

        // Read the matrix from the parent process
        read(parent_to_child[0], matrix, sizeof(Matrix));
        close(parent_to_child[0]);

        printf("Child process received matrix:\n");
        print_matrix(matrix);

        Vector* min = matrix_col_min(matrix);
        Vector* max = matrix_col_max(matrix);

        // Write the vectors' elements to the parent process
        
        for (int i = 0; i < max->size; i++) {
            write(child_to_parent[1], &(max->elements[i]), sizeof(double));
        }

        for (int i = 0; i < min->size; i++) {
            write(child_to_parent[1], &(min->elements[i]), sizeof(double));
        }
        close(child_to_parent[1]);
    exit(0);
        close(child_to_parent[1]);

        exit(0);
    } else {
        close(parent_to_child[0]);  // Close the read end of the parent-to-child pipe
        close(child_to_parent[1]);  // Close the write end of the child-to-parent pipe

        // Write the matrix to the child process
        write(parent_to_child[1], matrix, sizeof(Matrix));
        close(parent_to_child[1]);

        wait(NULL);

        Vector* max = create_vector(matrix->cols);
        Vector* min = create_vector(matrix->cols);

        // Read the vector elements from the child process
       for (int i = 0; i < max->size; i++) {
            read(child_to_parent[0], &(max->elements[i]), sizeof(double));
        }

        for (int i = 0; i < min->size; i++) {
            read(child_to_parent[0], &(min->elements[i]), sizeof(double));
        }
        close(child_to_parent[0]);

        printf("Parent process received vectors:\n");
        printf("Vector of maximum numbers:\n");
        print_vector(max);
        printf("Vector of minimum numbers:\n");
        print_vector(min);

        normalize_matrix_column_formula_1(matrix, max, min);

        printf("Normalized matrix:\n");
        print_matrix(matrix);
    }

    return 0;
}