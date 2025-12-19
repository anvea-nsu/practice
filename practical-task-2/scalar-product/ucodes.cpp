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

    for (int i = 0; i < part_size; ++i) {
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
    for (int i = 0; i < part_size; ++i) {
        result += px[i] * py[i];
    }

    z = result;
}

void init(DF &x, double val) {
    x = val;
}

} // extern "C"
