# Assignment 2: Blocked GEMM

## Description

GEMM operator: $C_{i,j}=\sum_{k} A_{i, k} \cdot B_{k,j}$

In Assignment 1, we design an HLS kernel for **complete** GEMM , which loads the matrixes $A$ and $B$ completely, calculates all elements of the matrix $C$, and stores the matrix back. However, when the matrixes ($A$, $B$, and $C$) have large scales, it's impossible to load all elements into the on-chip memory due to limited resources. Instead, we can load only partial elements of the matrixes $A$ and $B$ onto the chip, and calculate a **block** of matrix $C$.  

Formally, assuming that all the matrixes have size $N * N$, to calculate a block of matrix $C_{[l_0..r_0], [l_1..r_1]}$, **block** GEMM has the following steps:

1. load $A_{[l_0..r_0],[l_k..r_k]}$, and $B_{[l_k..r_k],[l_1..r_1]}$ into the chip memory

2. calculate $\forall i \in [l_0, r_0], j \in [l_1, r_1], C_{i,j}+=\sum_{k=1_k}^{r_k} A_{i, k} \cdot B_{k,j}$

3. repeat 1,2, until $l_k..r_k$ covers $[0..N-1]$

4. store $C_{[l_0..r_0],[l_1..r_1]}$ back to host

For Assignment 2, we want to design a blocked-GEMM HLS kernel, and use it with **PYNQ**. We will test the HLS kernel  by using it to accelerate GEMM of size $N*N$, where $N=1024$.  We have the following additional constraints:

* Your HLS kernel must use `hls::stream` data type and `axis` protocol for interface;
* The clock period is 10ns;
* The data type of the values in the input matrixes is `uint8` { [0, 256) }

## Instruction

TA recommends that you accelerate blocked GEMM following the steps below:

1. Design an HLS kernel, which accelerates the calculation of **$iB$ rows** of result matrix $C$;
2. Build a Vivado IP with the exported HLS IP, and deliver it onto the PYNQ board as an overlay.
3. Under the Jupyter environment of PYNQ,  prepare data, feed it into the overlay, and get results back **every $iB$ rows**.  After the calculation of the whole matrix C, validate the results, compare the execution time with `numpy.matmul()` and get your score according to "Section Evaluation".

Please watch the baseline tutorial video (`bgemm-tutorial`) carefully so that you understand it completely.

## Evaluation

Your score will be calculated as $\min(\lfloor \frac{\text{CPU-TIME}}{\text{FPGA-TIME}}\rfloor * 10, 100)$.

For example, if $\text{CPU-TIME}=180 \text{s}$ and $\text{FPGA-TIME}=19\text{s}$, you get score 90.

## Submission

Please submit `[name]-[id].zip` (without `[]`), which contains:

* HLS sources files (we don't require test bench files now)
* jupyter notebook file, which generates random inputs, call acceleration overlay, compare results and execution time with `numpy.matmul`.
  * Please ensure your jupyter notebook contains **outputs which demonstrate the correctness and performance of your design**.
* Report, which ***briefly*** describes your optimization techniques.

**DDL: 2022.04.08 23:59**

****
