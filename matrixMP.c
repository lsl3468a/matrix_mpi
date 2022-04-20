#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>
#include <time.h>

#define N 1000

int A[N][N], B[N][N], C[N][N], tA[N][N], tB[N][N], IA[N][N], IB[N][N];
int traceA, traceB, traceC, determinantA, determinantB;

int main()
{
    int i, j, k;
    struct timeval tv1, tv2, tvS, tvT;
    struct timezone tz;
    double elapsed;
    omp_set_num_threads(omp_get_num_procs());
    srand(time(NULL));

    gettimeofday(&tvS, &tz);
    // Remplissage des matrices
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
        {
            A[i][j] = rand() % 10;
            B[i][j] = rand() % 10;
        }

    traceA = A[0][0];
    traceB = B[0][0];

// Multiplication des matrices
gettimeofday(&tv1, &tz);
#pragma omp parallel for private(i, j, k) shared(A, B, C)
    for (i = 0; i < N; ++i)
    {
        for (j = 0; j < N; ++j)
        {
            for (k = 0; k < N; ++k)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    gettimeofday(&tv2, &tz);
    elapsed = (double)(tv2.tv_sec - tv1.tv_sec) + (double)(tv2.tv_usec - tv1.tv_usec) * 1.e-6;
    printf("elapsed time for multiplication = %f seconds.\n", elapsed);

    traceC = C[0][0];

// Transposition des matrices
gettimeofday(&tv1, &tz);
#pragma omp parallel for private(i, j) shared(A, B, tA, tB)
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            tA[j][i] = A[i][j];
            tB[j][i] = B[i][j];
        }
    }
    gettimeofday(&tv2, &tz);
    elapsed = (double)(tv2.tv_sec - tv1.tv_sec) + (double)(tv2.tv_usec - tv1.tv_usec) * 1.e-6;
    printf("elapsed time for transposition = %f seconds.\n", elapsed);

// Trace des matrices
gettimeofday(&tv1, &tz);
#pragma omp parallel for private(i) shared(A, B, C, traceA, traceB, traceC)
    for (int i = 1; i < N; i++)
    {
        traceA = traceA + A[i][i];
        traceB = traceB + B[i][i];
        traceC = traceC + C[i][i];
    }
    gettimeofday(&tv2, &tz);
    elapsed = (double)(tv2.tv_sec - tv1.tv_sec) + (double)(tv2.tv_usec - tv1.tv_usec) * 1.e-6;
    printf("elapsed time for trace = %f seconds.\n", elapsed);

//Inversion des matrices
gettimeofday(&tv1, &tz);
#pragma omp parallel for private(i,j) shared(A, B, determinantA, determinantB)
    for(int i = 0; i < N; i++){
            determinantA = determinantA + (A[0][i] * (A[1][(i+1)%3] * A[2][(i+2)%3] - A[1][(i+2)%3] * A[2][(i+1)%3]));
            determinantB = determinantB + (B[0][i] * (B[1][(i+1)%3] * B[2][(i+2)%3] - B[1][(i+2)%3] * B[2][(i+1)%3]));
    }
    for(int i = 0; i < N; i++){
            for(int j = 0; j < N; j++){
                IA[i][j] = ((A[(j+1)%3][(i+1)%3] * A[(j+2)%3][(i+2)%3]) - (A[(j+1)%3][(i+2)%3] * A[(j+2)%3][(i+1)%3]))/ determinantA;
                IB[i][j] = ((B[(j+1)%3][(i+1)%3] * B[(j+2)%3][(i+2)%3]) - (B[(j+1)%3][(i+2)%3] * B[(j+2)%3][(i+1)%3]))/ determinantB;
            }
        }
    gettimeofday(&tv2, &tz);
    elapsed = (double)(tv2.tv_sec - tv1.tv_sec) + (double)(tv2.tv_usec - tv1.tv_usec) * 1.e-6;
    printf("elapsed time for inversion = %f seconds.\n", elapsed);
    
    gettimeofday(&tvT, &tz);
    elapsed = (double)(tvT.tv_sec - tvS.tv_sec) + (double)(tvT.tv_usec - tvS.tv_usec) * 1.e-6;
    printf("elapsed time total = %f seconds.\n", elapsed);
}