#include "matrix.h"
#include <time.h>
#include <math.h>
#include <float.h>

Vector* create_vector(int size) {
    Vector* v = calloc(1, sizeof(Vector));
    v->size = size;
    v->elements = calloc(size, sizeof(double));
    return v;
}

Matrix* create_matrix(int rows, int cols) {
    Matrix* M = malloc(sizeof(Matrix));
    M->rows = rows;
    M->cols = cols;
    M->elements = calloc(rows, sizeof(double*));
    for (int i = 0; i < rows; ++i) {
        M->elements[i] = calloc(cols, sizeof(double));
    }

    return M;
}

Matrix* create_matrix_from_file(const char* file_path, int rows, int cols) {
    Matrix* M = create_matrix(rows, cols);
    FILE* fp = fopen(file_path, "r");
    if (fp == NULL) {
        fprintf(stderr, "Failed to open \"%s\". Not possible to create matrix from file.\n", file_path);
        return NULL;
    }

    double d;
    for (int i = 0; i < M->rows; ++i) {
        for (int j = 0; j < M->cols; ++j) {
            const int r = fscanf(fp, "%lf", &d);
            if (r != 1) {
                fprintf(stderr, "fscanf failed.\n");
                fclose(fp);
                return NULL;
            }
            M->elements[i][j] = d;
        }
    }

    fclose(fp);
    return M;
}

void free_matrix(Matrix* M) {
    for (int i = 0; i < M->rows; ++i) {
        free(M->elements[i]);
    }
    free(M->elements);
    free(M);
}

Vector* matrix_col_max(Matrix* M){
    Vector* v = create_vector(M->cols);
    for (int i = 0; i < M->cols; ++i) {
        double max = M->elements[0][i];
        for (int j = 0; j < M->rows; ++j) {
            if (max < M->elements[j][i]) {
                max = M->elements[j][i];
            }
        }
        v->elements[i] = max;
    }
    return v; 
}

Vector* matrix_col_min(Matrix* M){
    Vector* v = create_vector(M->cols);
    for (int i = 0; i < M->cols; ++i) {
        double min = M->elements[0][i];
        for (int j = 0; j < M->rows; ++j) {
            if (min > M->elements[j][i]) {
                min = M->elements[j][i];
            }
        }
        v->elements[i] = min;
    }
    return v; 
}

void print_matrix(const Matrix* M) {
    printf("rows=%d,cols=%d,\n[\n", M->rows, M->cols);
    for (int i = 0; i < M->rows; ++i) {
        printf("[");
        for (int j = 0; j < M->cols; ++j) {
            printf("%lf ", M->elements[i][j]);
        }
        printf("]\n");
    }
    printf("]\n");
}
void normalize_matrix_column_formula_1(Matrix* M, Vector* mayores, Vector* menores) {
    for (int i = 0; i < M->rows; ++i) {
        for (int j = 0; j < M->cols; ++j) {
            M->elements[i][j] = (M->elements[i][j] - menores->elements[j]) / (mayores->elements[j] - menores->elements[j]);
        }
    }
}
void print_vector(const Vector* v) {
    printf("size=%d, [", v->size);
    for (int i = 0; i < v->size; ++i) {
        printf("%lf ", v->elements[i]);
    }
    printf("]\n");
}