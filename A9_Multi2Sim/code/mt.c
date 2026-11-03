#include <stdio.h>
#include <pthread.h>
#define N 32
#ifndef THREADS
#define THREADS 1
#endif
int A[N][N], B[N][N], C[N][N];

void *multiply(void *arg) {
    int tid = *(int *)arg;
    int start = tid * N / THREADS;
    int end = (tid + 1) * N / THREADS;
    for (int i = start; i < end; i++)
        for (int j = 0; j < N; j++)
            for (int k = 0; k < N; k++)
                C[i][j] += A[i][k] * B[k][j];
    return NULL;
}

int main(void) {
    pthread_t threads[THREADS];
    int ids[THREADS];
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            A[i][j] = 1; B[i][j] = 2; C[i][j] = 0;
        }
    for (int t = 0; t < THREADS; t++) {
        ids[t] = t;
        if (pthread_create(&threads[t], NULL, multiply, &ids[t]) != 0) {
            fprintf(stderr, "Thread creation failed\n");
            return 1;
        }
    }
    for (int t = 0; t < THREADS; t++)
        pthread_join(threads[t], NULL);
    printf("Done: %d\n", C[0][0]);
    return 0;
}
