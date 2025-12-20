#ifndef UCODES_H
#define UCODES_H

#include "ucenv.h"

#ifdef __cplusplus
extern "C" {
#endif

void init_vector_part(int vector_id, int part_number,
						int part_size, DF &vector);
void print_result(double n);
void smulv2_part(const DF &x, const DF &y, DF &z, int part_size);
void init(DF &x, double val);

#ifdef __cplusplus
}
#endif

#endif // UCODES_H
