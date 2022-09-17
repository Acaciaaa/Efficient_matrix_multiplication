// -----------------------------|
// Don't modify these macros ---|
#define eps 1e-6
#define I 64
#define J 64
#define K 64
// -----------------------------|

#include "ap_fixed.h"
typedef ap_ufixed<8, 5> in_t;
typedef ap_ufixed<22, 16> data_t;


void mm(in_t A[I][K], in_t B[K][J], data_t C[I][J]);
