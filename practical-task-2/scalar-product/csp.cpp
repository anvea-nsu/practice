#include "ucodes.h"

#define VECTOR_PART_SIZE 1000
#define PARTS_COUNT 10

void reduce_sum(DF *arr, int begin, int end, DF &result) {
    if (end - begin == 1) {
        init(result, arr[begin]);
        return;
    }

    if (end - begin > 1) {
        DF tmp1, tmp2;
        int mid = begin + (end - begin) / 2;

        reduce_sum(arr, begin, mid, tmp1);
        reduce_sum(arr, mid, end, tmp2);

        init(result, tmp1.getData<double>()[0]
                   + tmp2.getData<double>()[0]);
    }
}

void mulv(DF *x, DF *y, DF *z) {
    for (int i = 0; i < PARTS_COUNT; ++i) {
        smulv2_part(x[i], y[i], z[i], VECTOR_PART_SIZE);
    }
}

int main() {
    DF x[PARTS_COUNT];
    DF y[PARTS_COUNT];
    DF z[PARTS_COUNT];
    DF result;

    for (int i = 0; i < PARTS_COUNT; ++i) {
        init_vector_part(0, i, VECTOR_PART_SIZE, x[i]);
        init_vector_part(1, i, VECTOR_PART_SIZE, y[i]);
    }

    mulv(x, y, z);

    reduce_sum(z, 0, PARTS_COUNT, result);

    print_result(result.getData<double>()[0]);

    return 0;
}
