#include "bgemm.h"
#include <assert.h>
#include <cstring>

template <class T_STREAM, int I, int K>
void load(T_STREAM &in, in_t buf[I][K]){
#pragma HLS pipeline II=1
	for(int i = 0; i < I; i++){
		auto x = in.read();
		for(int k = 0; k < K; k++){
			buf[i][k] = x.data.x[k];
		}
	}
}

void compute(in_t buf_A[iB][kB], in_t buf_B[jB][kB], out_t acc_C[iB][jB]){
#pragma HLS array_partition variable=buf_A dim=2 cyclic factor=16
#pragma HLS array_partition variable=buf_B dim=2 cyclic factor=16
#pragma HLS array_partition variable=acc_C dim=2 cyclic factor=16
	for(int i = 0; i < iB; i++)
#pragma HLS pipeline II=1
		for(int j = 0; j < jB; j++){
#pragma HLS unroll
			for(int k = 0; k < 16; k++)
				acc_C[i][j] += buf_A[i][k] * buf_B[j][k];
		}
}

void store(out_t acc_C[iB][jB], hls::stream<axis_out_t> &out_C, bool last){
	for(int i = 0; i < iB; i++){
		axis_out_t x;
		for(int j = 0; j < jB; j++){
			x.data.x[j] = acc_C[i][j];
			acc_C[i][j] = 0;
		}
		x.last = last && (i+1==iB);
		x.keep = -1;
		out_C.write(x);
	}
}

void bgemm(hls::stream<axis_in_a_t> &in_A, hls::stream<axis_in_b_t> &in_B, hls::stream<axis_out_t> &out_C) {
#pragma HLS INTERFACE s_axilite port=return
#pragma HLS INTERFACE axis port=in_A
#pragma HLS INTERFACE axis port=in_B
#pragma HLS INTERFACE axis port=out_C
	constexpr int I = N / jB;
	constexpr int J = N / kB;
	in_t buf_A[iB][kB], buf_B[jB][kB];
	out_t acc_C[iB][jB];
	for(int i = 0; i < iB; i++)
		for(int j = 0; j < jB; j++)
			acc_C[i][j] = 0;

	for(int i = 0; i < I; i++){
		for(int j = 0; j < J; j++){
#pragma HLS dataflow
			load<typename hls::stream<axis_in_a_t>, iB, kB>(in_A, buf_A);
			load<typename hls::stream<axis_in_b_t>, jB, kB>(in_B, buf_B);
			compute(buf_A, buf_B, acc_C);
		}
		store(acc_C, out_C, (i+1) == I);
	}

//	bool end = 0;
//	int count = 0;
//	out_t sum;
//
//	in_t buf_A[iB][kB]={0}, buf_B[jB][kB]={0};
//	out_t buf_C[iB][jB];
//
//	memset(buf_C, 0, sizeof(buf_C));
//
//	while (!end) {
//#pragma HLS pipeline
//		bool a_end=0, b_end=0;
//		for (int i=0; i<iB; i++)
//			for (int k=0; k<kB; k++) {
//				auto x = in_A.read();
//				a_end |= x.last;
//				buf_A[i][k] = x.data;
//			}
//		for (int j=0; j<jB; j++)
//			for (int k=0; k<kB; k++) {
//				auto x = in_B.read();
//				b_end |= x.last;
//				buf_B[j][k] = x.data;
//			}
//
//		assert(a_end == b_end);
//		end = a_end;
//
//		for (int i=0; i<iB; i++)
//			for (int j=0; j<jB; j++) {
//				sum = 0;
//				for (int k=0; k<kB; k++)
//					sum += buf_A[i][k] * buf_B[j][k];
//				buf_C[i][j] += sum;
//			}
//		count += kB;
//
//		if (count == N) {
//			count = 0;
//			for (int i=0; i<iB; i++)
//				for (int j=0; j<jB; j++) {
//					axis_out_t x;
//					x.data = buf_C[i][j];
//					x.last = end && (i+1==iB) && (j+1==jB);
//					x.keep = -1;
//					out.write(x);
//					buf_C[i][j] = 0;
//				}
//		}
//	}
}

