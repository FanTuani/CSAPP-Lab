/*
 * trans.c - 矩阵转置 B = A^T
 *
 * 每个转置函数的原型应为:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * 转置函数的评估基于在 1KB 直接映射缓存（块大小 32 字节）上的未命中次数。
 */
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/*
 * transpose_submit - 这是你在作业第 B 部分要被评分的转置函数。
 *     不要修改描述字符串 "Transpose submission"，因为驱动程序会
 *     通过该字符串来识别要评分的转置函数。
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int m, int n, int a[n][m], int b[m][n]) {
    if (n == 32 && m == 32) {
        const int step = 8;
        for (int i = 0; i < 32; i += step) {
            for (int j = 0; j < 32; j += step) {
                for (int x = 0; x < step; x++) {
                    int temp1 = a[i + x][j];
                    int temp2 = a[i + x][j + 1];
                    int temp3 = a[i + x][j + 2];
                    int temp4 = a[i + x][j + 3];
                    int temp5 = a[i + x][j + 4];
                    int temp6 = a[i + x][j + 5];
                    int temp7 = a[i + x][j + 6];
                    int temp8 = a[i + x][j + 7];

                    b[j][i + x] = temp1;
                    b[j + 1][i + x] = temp2;
                    b[j + 2][i + x] = temp3;
                    b[j + 3][i + x] = temp4;
                    b[j + 4][i + x] = temp5;
                    b[j + 5][i + x] = temp6;
                    b[j + 6][i + x] = temp7;
                    b[j + 7][i + x] = temp8;
                }
            }
        }
    } else if (n == 64 && m == 64) {
        int a_0, a_1, a_2, a_3, a_4, a_5, a_6, a_7;
        for (int i = 0; i < 64; i += 8) {
            for (int j = 0; j < 64; j += 8) {
                for (int k = i; k < i + 4; k++) {
                    // 得到A的第1,2块
                    a_0 = a[k][j + 0];
                    a_1 = a[k][j + 1];
                    a_2 = a[k][j + 2];
                    a_3 = a[k][j + 3];
                    a_4 = a[k][j + 4];
                    a_5 = a[k][j + 5];
                    a_6 = a[k][j + 6];
                    a_7 = a[k][j + 7];
                    // 复制给B的第1,2块
                    b[j + 0][k] = a_0;
                    b[j + 1][k] = a_1;
                    b[j + 2][k] = a_2;
                    b[j + 3][k] = a_3;
                    b[j + 0][k + 4] = a_4;
                    b[j + 1][k + 4] = a_5;
                    b[j + 2][k + 4] = a_6;
                    b[j + 3][k + 4] = a_7;
                }
                for (int k = j; k < j + 4; k++) {
                    // 得到B的第2块
                    a_0 = b[k][i + 4];
                    a_1 = b[k][i + 5];
                    a_2 = b[k][i + 6];
                    a_3 = b[k][i + 7];
                    // 得到A的第3块
                    a_4 = a[i + 4][k];
                    a_5 = a[i + 5][k];
                    a_6 = a[i + 6][k];
                    a_7 = a[i + 7][k];
                    // 复制给B的第2块
                    b[k][i + 4] = a_4;
                    b[k][i + 5] = a_5;
                    b[k][i + 6] = a_6;
                    b[k][i + 7] = a_7;
                    // B原来的第2块移动到第3块
                    b[k + 4][i + 0] = a_0;
                    b[k + 4][i + 1] = a_1;
                    b[k + 4][i + 2] = a_2;
                    b[k + 4][i + 3] = a_3;
                }
                for (int k = i + 4; k < i + 8; k++) {
                    // 处理第4块
                    a_4 = a[k][j + 4];
                    a_5 = a[k][j + 5];
                    a_6 = a[k][j + 6];
                    a_7 = a[k][j + 7];
                    b[j + 4][k] = a_4;
                    b[j + 5][k] = a_5;
                    b[j + 6][k] = a_6;
                    b[j + 7][k] = a_7;
                }
            }
        }
    } else {
        for (int i = 0; i < n; i += 16)
            for (int j = 0; j < m; j += 16)
                for (int k = i; k < i + 16 && k < n; ++k)
                    for (int l = j; l < j + 16 && l < m; ++l)
                        b[l][k] = a[k][l];
    }
}
/*
 * 你可以在下面定义额外的转置函数。我们下面提供了一个
 * 简单的示例来帮助你入门。
 */

/*
 * trans - 一个简单的基线转置函数，未针对缓存进行优化。
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N]) {
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }
}

/*
 * registerFunctions - 此函数将你的转置函数注册到驱动中。
 *     在运行时，驱动会评估所有注册的函数并总结它们的性能。
 *     这是试验不同转置策略的一个方便方法。
 */
void registerFunctions() {
    /* 注册你的解题函数 */
    registerTransFunction(transpose_submit, transpose_submit_desc);

    /* 注册其他转置函数（如有） */
    registerTransFunction(trans, trans_desc);
}

/*
 * is_transpose - 该辅助函数用于检查 B 是否为 A 的转置。
 *     你可以在转置函数返回前调用它来验证转置结果的正确性。
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N]) {
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
