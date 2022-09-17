#include "matmul.h"
#include <cstring>
void copy_C(data_t* local_C, data_t C[J]){
#pragma HLS inline off
	for(int j = 0; j < I; j++){
#pragma HLS pipeline II=1
		C[j] = local_C[j];
	}
}
// Match mm's function declaration in matmul.h
void mm(in_t A[I][K], in_t B[K][J], data_t C[I][J]) {

#pragma HLS interface ap_ctrl_none port = return

  // You must use m_axi interface for A, B, C,
  // but you could add additional attributes.
  // You may refer to
  // https://docs.xilinx.com/r/en-US/ug1399-vitis-hls/Optimizing-AXI-System-Performance
  // for more information

#pragma HLS INTERFACE mode=m_axi port=A
#pragma HLS INTERFACE mode=m_axi port=B
#pragma HLS INTERFACE mode=m_axi port=C

  in_t local_B[K][J];
  data_t local_C[I][J];
#pragma HLS stream variable=local_C off depth=1

  for (int i = 0; i < I; i++)
#pragma HLS pipeline II=1
	  memcpy(&local_B[i], &B[i], 64 * sizeof(in_t));

#pragma HLS array_partition variable=local_B dim=2 cyclic factor=16
#pragma HLS array_partition variable=local_C dim=2 cyclic factor=16

  for(int x = 0; x < I; x++){
	  for(int j = 0; j < J; j++){
		for(int i = 0; i < I; i+=16){
#pragma HLS pipeline II=1
			for(int k = 0; k < 16; k++)
#pragma HLS unroll
			  local_C[x][i+k] += A[x][j] * local_B[j][i+k];
		}
	  }
#pragma HLS dataflow
	  copy_C(local_C[x], C[x]);
  }
}
