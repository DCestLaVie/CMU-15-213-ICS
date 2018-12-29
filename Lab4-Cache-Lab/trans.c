/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

void transpose_64_64(int M, int N, int A[N][M], int B[M][N]);
void transpose_M_N(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    if(M == 64 && N == 64) {
        transpose_64_64(M, N, A, B);
    } else {
        transpose_M_N(M, N, A, B);
    }
}

void transpose_64_64(int M, int N, int A[N][M], int B[M][N]) {
    int temp_0, temp_1, temp_2, temp_3;
    int temp_4, temp_5, temp_6, temp_7;
    int i, j, k;
    for(i = 0; i < 64; i += 8) {
        for(j = 0; j < 64; j += 8) {
            for(k = i; k < i + 4; ++k) {
                temp_0 = A[k][j];
                temp_1 = A[k][j+1];
                temp_2 = A[k][j+2];
                temp_3 = A[k][j+3];
                temp_4 = A[k][j+4];
                temp_5 = A[k][j+5];
                temp_6 = A[k][j+6];
                temp_7 = A[k][j+7];

                B[j][k] = temp_0;
                B[j+1][k] = temp_1;
                B[j+2][k] = temp_2;
                B[j+3][k] = temp_3;
                B[j][k+4] = temp_7;
                B[j+1][k+4] = temp_6;
                B[j+2][k+4] = temp_5;
                B[j+3][k+4] = temp_4;
            }
            for(k = 0; k < 4; ++k) {
                temp_0 = A[i+4][j+3-k];
                temp_1 = A[i+5][j+3-k];
                temp_2 = A[i+6][j+3-k];
                temp_3 = A[i+7][j+3-k];
                temp_4 = A[i+4][j+4+k];
                temp_5 = A[i+5][j+4+k];
                temp_6 = A[i+6][j+4+k];
                temp_7 = A[i+7][j+4+k];

                B[j+4+k][i] = B[j+3-k][i+4];
                B[j+4+k][i+1] = B[j+3-k][i+5];
                B[j+4+k][i+2] = B[j+3-k][i+6];
                B[j+4+k][i+3] = B[j+3-k][i+7];
                B[j+4+k][i+4] = temp_4;
                B[j+4+k][i+5] = temp_5;
                B[j+4+k][i+6] = temp_6;
                B[j+4+k][i+7] = temp_7;
                B[j+3-k][i+4] = temp_0;
                B[j+3-k][i+5] = temp_1;
                B[j+3-k][i+6] = temp_2;
                B[j+3-k][i+7] = temp_3;
            }
        }
    }
}

void transpose_M_N(int M, int N, int A[N][M], int B[M][N]) {
    int i, j, k, m;
    int temp, diagonal = -1;
    int per_row = 16, per_col = 8;
    for(i = 0; i < N; i += per_row) {
        for(j = 0; j < M; j += per_col) {
            for(k = i; k < N && k < i + per_row; ++k) {
                for(m = j; m < M && m < j + per_col; ++m) {
                    if(m == k) {
                        diagonal = m;
                        temp = A[k][m];
                    } else {
                        B[m][k] = A[k][m];
                    }
                }
                if(diagonal != -1) {
                    B[diagonal][diagonal] = temp;
                    diagonal = -1;
                }
            }
        }
    }
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

