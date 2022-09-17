#include "ap_int.h"
#include "ap_axi_sdata.h"
#include "hls_stream.h"

#define N 1024
#define iB 16
#define jB 16
#define kB 16

typedef ap_uint<8> in_t;
typedef ap_uint<32> out_t;

typedef struct{
	in_t x[iB];
}pack_a_t;
typedef struct{
	in_t x[jB];
}pack_b_t;
typedef struct{
	out_t x[jB];
}pack_c_t;

typedef hls::axis<pack_a_t, 0, 0, 0> axis_in_a_t;
typedef hls::axis<pack_b_t, 0, 0, 0> axis_in_b_t;
typedef hls::axis<pack_c_t, 0, 0, 0> axis_out_t;

void bgemm(hls::stream<axis_in_a_t> &, hls::stream<axis_in_b_t> &, hls::stream<axis_out_t> &);


