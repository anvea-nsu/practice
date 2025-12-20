#include "ucodes.h"

#define MATRIX_SIZE 1000
#define PARTS_COUNT 10
#define PART_ROWS (MATRIX_SIZE / PARTS_COUNT)

int main() {
    DF matrix[PARTS_COUNT];
    DF result[PARTS_COUNT];
    DF vector;

    for (int i = 0; i < PARTS_COUNT; i++) {
        init_matrix_part(
            i,
            PART_ROWS,
            MATRIX_SIZE,
            matrix[i]
        );
    }

    init_vector(MATRIX_SIZE, vector);

    for (int i = 0; i < PARTS_COUNT; i++) {
        matrix_vector_mult_part(
            matrix[i],
            vector,
            result[i],
            PART_ROWS,
            MATRIX_SIZE
        );
    }

    for (int i = 0; i < PARTS_COUNT; i++) {
        print_result_part(
            i,
            result[i],
            PART_ROWS
        );
    }

    return 0;
}
