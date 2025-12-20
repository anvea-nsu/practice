#ifndef UCODES_H
#define UCODES_H

#include "ucenv.h"

#ifdef __cplusplus
extern "C" {
#endif

void init_vector_part(
    int vector_id,
    int part_number,
    int part_size,
    DF &vector
);

void print_result(double n);

void smulv2_part(
    const DF &x,
    const DF &y,
    DF &z,
    int part_size
);

void init(DF &x, double val);

void init_matrix_part(
    int part_number,
    int rows,
    int cols,
    DF &matrix_part
);

void init_vector(
    int size,
    DF &vector
);

void matrix_vector_mult_part(
    const DF &matrix_part,
    const DF &vector,
    DF &result,
    int rows,
    int cols
);

void print_result_part(
    int part_number,
    const DF &result_part,
    int size
);

#ifdef __cplusplus
}
#endif

#endif // UCODES_H
