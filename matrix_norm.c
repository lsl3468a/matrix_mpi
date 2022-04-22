#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#define N 1000

double matrix_a[N][N],matrix_b[N][N],matrix_c[N][N], matrix_i[N][N], matrix_tA[N][N], matrix_tB[N][N];
double t1, t2, ts, tt;

int main(){
    int traceA, traceB, traceC, i, j = 0;
    float determinant = 0;

    ts = clock();
    srand ( time(NULL) );
        for (int i = 0; i<N; i++) {
            for (int j = 0; j<N; j++) {
                matrix_a[i][j]= rand()%10;
                matrix_b[i][j]= rand()%10;
            }
        }
    
    t1 = clock();
    for (int k = 0; k<N; k++) {
            for (int i = 0; i<N; i++) {
                matrix_c[i][k] = 0.0;
                for (int j = 0; j<N; j++)
                matrix_c[i][k] = matrix_c[i][k] + matrix_a[i][j] * matrix_b[j][k];
            }
        }
    t2 = clock();
    printf("Temps total calcul multiplication %f \n",  (t2 - t1) * 1e-6);

    //Calcul des traces des matrices
    t1 = clock();
    traceA = matrix_a[0][0];
    for(int i=1; i<N; i++){
        traceA = traceA + matrix_a[i][i];
    }
        
    traceB = matrix_b[0][0];
    for(int i=1; i<N; i++){
        traceB = traceB + matrix_b[i][i];
    }

    traceC = matrix_c[0][0];
    for(int i=1; i<N; i++){
        traceC = traceC + matrix_c[i][i];
    }
    t2 = clock();
    printf("Temps total calcul traces %f \n",  (t2 - t1) * 1e-6);

    //Inversion des matrices
    t1 = clock();
    for(i = 0; i < N; i++){
            determinant = determinant + (matrix_a[0][i] * (matrix_a[1][(i+1)%3] * matrix_a[2][(i+2)%3] - matrix_a[1][(i+2)%3] * matrix_a[2][(i+1)%3]));
        }

    for(i = 0; i < N; i++){
        for(j = 0; j < N; j++){
            matrix_i[i][j] = ((matrix_a[(j+1)%3][(i+1)%3] * matrix_a[(j+2)%3][(i+2)%3]) - (matrix_a[(j+1)%3][(i+2)%3] * matrix_a[(j+2)%3][(i+1)%3]))/ determinant;
       }
   }
   t2 = clock();
   printf("Temps total calcul inversion %f \n",  (t2 - t1) * 1e-6);

    t1 = clock();
   //Transposée des matrices
        for(int i=0; i<N; i++){
          for(int j=0; j<N; j++){
            matrix_tA[j][i] = matrix_a[i][j];
            matrix_tB[j][i] = matrix_b[i][j];
          }
        }
    t2 = clock();
    printf("Temps total calcul transposition %f \n",  (t2 - t1) * 1e-6);

    tt = clock();
    printf("Temps total execution %f \n",  (tt - ts) * 1e-6);

}