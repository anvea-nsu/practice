#include "ucodes.h"
#include <cstdio>
#include <cmath>

extern "C" {

void init_vector_part(
    int vector_id,
    int part_number,
    int part_size,
    DF &vector
) {
    double *data = vector.create<double>(part_size);

    for (int i = 0; i < part_size; i++) {
        if (vector_id == 0)
            data[i] = part_number * part_size + i;
        else
            data[i] = (part_number * part_size + i) * 0.1;
    }
}

void print_result(double n) {
    printf("Result: %lf\n", sqrt(n));
}

void smulv2_part(
    const DF &x,
    const DF &y,
    DF &z,
    int part_size
) {
    const double *px = x.getData<double>();
    const double *py = y.getData<double>();

    double result = 0.0;
    for (int i = 0; i < part_size; i++) {
        result += px[i] * py[i];
    }

    z = result;
}

void init(DF &x, double val) {
    x = val;
}

void init_matrix_part(
    int part_number,
    int rows,
    int cols,
    DF &matrix_part
) {
    double *data = matrix_part.create<double>(rows * cols);

    for (int i = 0; i < rows; i++) {
        int global_row = part_number * rows + i;
        for (int j = 0; j < cols; j++) {
            data[i * cols + j] = global_row + j * 0.1;
        }
    }
}

void init_vector(
    int size,
    DF &vector
) {
    double *data = vector.create<double>(size);
    for (int i = 0; i < size; i++) {
        data[i] = i * 0.5;
    }
}

void matrix_vector_mult_part(
    const DF &matrix_part,
    const DF &vector,
    DF &result,
    int rows,
    int cols
) {
    const double *mat = matrix_part.getData<double>();
    const double *vec = vector.getData<double>();
    double *res = result.create<double>(rows);

    for (int i = 0; i < rows; i++) {
        res[i] = 0.0;
        for (int j = 0; j < cols; j++) {
            res[i] += mat[i * cols + j] * vec[j];
        }
    }
}

void print_result_part(
    int part_number,
    const DF &result_part,
    int size
) {
    const double *data = result_part.getData<double>();

    printf("Result part %d:\n", part_number);
    for (int i = 0; i < size && i < 5; i++) {
        printf("  [%d] = %lf\n", part_number * size + i, data[i]);
    }
    if (size > 5) {
        printf("  ...\n");
    }
}

} // extern "C"
