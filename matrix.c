#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"
#include <time.h>
#define N 1000

MPI_Status status;
double matrix_a[N][N],matrix_b[N][N],matrix_c[N][N], matrix_i[N][N], matrix_tA[N][N], matrix_tB[N][N];
double end, endC, start, startC;

int main(int argc, char **argv)
{
    int size, rank, nbslave, source, i, j, k, lignes, offset;
    int traceA, traceB, traceC = 0;
    float determinant = 0;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    nbslave = size - 1;
    start = MPI_Wtime();


    if (rank == 0) {
        
    // Remplir les matrices A et B avec des nombres aléatoires
        srand ( time(NULL) );
        for (int i = 0; i<N; i++) {
            for (int j = 0; j<N; j++) {
                matrix_a[i][j]= rand()%10;
                matrix_b[i][j]= rand()%10;
            }
        }
	
  printf("Opérations sur les matrices : \n");

    /* // Print Matrix A
    printf("\nMatrice A\n\n");
    for (int i = 0; i<N; i++) {
      for (int j = 0; j<N; j++) {
        printf("%.0f\t", matrix_a[i][j]);
      }
	    printf("\n");
    }

    // Print Matrix B
    printf("\nMatrice B\n\n");
    for (int i = 0; i<N; i++) {
      for (int j = 0; j<N; j++) {
        printf("%.0f\t", matrix_b[i][j]);
      }
	    printf("\n");
    }
 */
    //Nombre de lignes envoyées à chaque processus
    lignes = N/nbslave;
    offset = 0;

    
    //Envoi aux esclaves les informations
    for (int i=1; i <= nbslave; i++){
      MPI_Send(&offset, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
      MPI_Send(&lignes, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
      MPI_Send(&matrix_a[offset][0], lignes*N, MPI_DOUBLE,i,1, MPI_COMM_WORLD);
      MPI_Send(&matrix_b, N*N, MPI_DOUBLE, i, 1, MPI_COMM_WORLD);
      offset = offset + lignes;
    }

    //Réception des résultats
    for (int i = 1; i <= nbslave; i++){
      source = i;
      MPI_Recv(&offset, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
      MPI_Recv(&lignes, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
      MPI_Recv(&matrix_c[offset][0], lignes*N, MPI_DOUBLE, source, 2, MPI_COMM_WORLD, &status);
      MPI_Recv(&traceA, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
      MPI_Recv(&traceB, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
      MPI_Recv(&traceC, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
      MPI_Recv(&matrix_tA[offset][0], lignes*N, MPI_DOUBLE, source, 2, MPI_COMM_WORLD, &status);
      MPI_Recv(&matrix_tB[offset][0], lignes*N, MPI_DOUBLE, source, 2, MPI_COMM_WORLD, &status);
      MPI_Recv(&matrix_i[offset][0], lignes*N, MPI_DOUBLE, source, 2, MPI_COMM_WORLD, &status);
    }

    /* // Impression des résultats
    printf("\nResultats matrice C = Matrice A * Matrice B:\n\n");
    for (int i = 0; i<N; i++) {
      for (int j = 0; j<N; j++)
        printf("%.0f\t", matrix_c[i][j]);
      printf ("\n");
    }
    printf ("\n");

    //Impression des traces
    printf("\nTrace de la matrice A :\n\n");
    printf("%d\n", traceA);

    printf("\nTrace de la matrice B :\n\n");
    printf("%d\n", traceB);

    printf("\nTrace de la matrice C :\n\n");
    printf("%d\n", traceC);

    // Impression des résultats
    printf("\nResultats matrice Transposée de Matrice A :\n\n");
    for (int i = 0; i<N; i++) {
      for (int j = 0; j<N; j++)
        printf("%.0f\t", matrix_tA[i][j]);
      printf ("\n");
    }
    printf ("\n");

    printf("\nMatrice inverse de A :\n\n");
    for(i=0; i<N; i++){
      for(j=0; j<N; j++){
          printf("%.0f\t", matrix_i[i][j]);
      }
      printf ("\n");
    }
    printf ("\n");

    // Impression des résultats
    printf("\nResultats matrice Transposée de Matrice B :\n\n");
    for (int i = 0; i<N; i++) {
      for (int j = 0; j<N; j++)
        printf("%.0f\t", matrix_tB[i][j]);
      printf ("\n");
    }
    printf ("\n"); */
  }

    // Processus esclaves
    if (rank > 0) {
        source = 0;
        MPI_Recv(&offset, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&lignes, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&matrix_a, lignes*N, MPI_DOUBLE, source, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&matrix_b, N*N, MPI_DOUBLE, source, 1, MPI_COMM_WORLD, &status);
        
        startC = MPI_Wtime();
        //Multiplication des matrices
        for (int k = 0; k<N; k++) {
            for (int i = 0; i<lignes; i++) {
                matrix_c[i][k] = 0.0;
                for (int j = 0; j<N; j++)
                matrix_c[i][k] = matrix_c[i][k] + matrix_a[i][j] * matrix_b[j][k];
            }
        }
        endC = MPI_Wtime();
        printf("Temps de calcul Multiplication %f\n", endC-startC);

        startC = MPI_Wtime();
        //Trace des matrices
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
        endC = MPI_Wtime();
        printf("Temps de calcul des Traces %f\n", endC-startC);

        startC = MPI_Wtime();
        //finding determinant
        for(i = 0; i < 3; i++){
            determinant = determinant + (matrix_a[0][i] * (matrix_a[1][(i+1)%3] * matrix_a[2][(i+2)%3] - matrix_a[1][(i+2)%3] * matrix_a[2][(i+1)%3]));
        }

        for(i = 0; i < 3; i++){
            for(j = 0; j < 3; j++){
                matrix_i[i][j] = ((matrix_a[(j+1)%3][(i+1)%3] * matrix_a[(j+2)%3][(i+2)%3]) - (matrix_a[(j+1)%3][(i+2)%3] * matrix_a[(j+2)%3][(i+1)%3]))/ determinant;
            }
        }
        endC = MPI_Wtime();
        printf("Temps de calcul de l'inversion %f\n", endC-startC);
        
        startC = MPI_Wtime();
        //Transposée des matrices
        for(int i=0; i<N; i++){
          for(int j=0; j<N; j++){
            matrix_tA[j][i] = matrix_a[i][j];
            matrix_tB[j][i] = matrix_b[i][j];
          }
        }
        endC = MPI_Wtime();
        printf("Temps de calcul de la transposee %f\n", endC-startC);

        //Envoi des résultats
        MPI_Send(&offset, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
        MPI_Send(&lignes, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
        MPI_Send(&matrix_c, lignes*N, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
        MPI_Send(&traceA, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
        MPI_Send(&traceB, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
        MPI_Send(&traceC, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
        MPI_Send(&matrix_tA, lignes*N, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
        MPI_Send(&matrix_tB, lignes*N, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
        MPI_Send(&matrix_i, lignes*N, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
    }
  end = MPI_Wtime();
  printf("Temps de calcul total de %d est de %f\n", rank, end-start);
  MPI_Finalize();
}
