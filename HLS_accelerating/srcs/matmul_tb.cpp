#include "matmul.h"
#include <cmath>
#include <fstream>
#include <iostream>

int main() {
  double A[I][K], B[K][J];
  double C_ans[I][J];

  // You may modify kernel-related codes (like A_in, B_in, and etc.).
  // But don't modify the things about the ground-truth (A, B, C_ans).

  in_t A_in[I][K], B_in[K][J];

  data_t C[I][J];

  std::ifstream in("mat.txt");

  for (int i = 0; i < I; i++)
    for (int k = 0; k < K; k++) {
      in >> A[i][k];
      A_in[i][k] = A[i][k];
    }

  for (int k = 0; k < K; k++)
    for (int j = 0; j < J; j++) {
      in >> B[k][j];
      B_in[k][j] = B[k][j];
    }

  mm(A_in, B_in, C);

  // Don't modify the code below! ---------------------------------------------

  for (int i = 0; i < I; i++)
    for (int j = 0; j < J; j++) {
      C_ans[i][j] = 0;
      for (int k = 0; k < K; k++) {
        C_ans[i][j] += A[i][k] * B[k][j];
      }
    }

  for (int i = 0; i < I; i++)
    for (int j = 0; j < J; j++)
      if ((double)C[i][j] - C_ans[i][j] > eps ||
          C_ans[i][j] - (double)C[i][j] > eps) {
        std::cout << "too much error at (" << i << ", " << j << ")"
                  << std::endl;
        return 1;
      }
  std::cout << "Pass test" << std::endl;
  return 0;
}
