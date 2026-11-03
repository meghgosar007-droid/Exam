#include <stdio.h>
#define N 32
int A[N][N], B[N][N], C[N][N];
int main() {
    int i, j, k;
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++) { A[i][j] = 1; B[i][j] = 2; C[i][j] = 0; }
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            for (k = 0; k < N; k++)
                C[i][j] += A[i][k] * B[k][j];
    printf("Done: %d\n", C[0][0]);
    return 0;
}
