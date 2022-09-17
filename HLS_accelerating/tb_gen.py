# Don't modify this code script.
import os
import sys
import numpy as np

if __name__=="__main__":
  I, J, K = 64, 64, 64
  assert(len(sys.argv)==2)
  dir = os.path.join(os.getcwd(), sys.argv[1])
  if not os.path.exists(dir):
    os.mkdir(dir)
  with open(os.path.join(dir, "mat.txt"), 'w') as f:
    def mkMat(r, c):
      A = np.trunc(np.random.rand(r, c) * (1<<8)) / (1<<3)
      return A, [' '.join(map(str, row))+'\n' for row in A]
    (A, A_str), (B, B_str) = mkMat(I, K), mkMat(J, K)

    f.writelines(A_str)
    f.writelines(B_str)
