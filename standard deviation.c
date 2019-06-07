#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include<time.h>
#include "mpi.h"

int main(int argc, char* argv[])
{
    int i, rank, noPro;
    int tag = 0;
    MPI_Init(&argc, &argv);
    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &noPro);

    int n;
    if(rank == 0)
    {
    printf("Enter number of data : ");
    scanf("%d", &n);
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int localSum1 = 0;
    int arr[n];
    srand(time(0));
    for(i=0; i<n; i++)
    {
        int val = (rand() % (50 - 0 + 1));
        printf("\nvalue :%d ", val);
        localSum1 += val;
        arr[i] = val;
    }
    printf("\nlocalSum1 %d ", localSum1);

    int globalSum1;
    MPI_Allreduce(&localSum1, &globalSum1, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    double globalMean1 = (double)globalSum1/(n*noPro);

    printf("\nglobalSum1 %d ", globalSum1);
    printf("\nglobalMean1 %16f ", globalMean1);

    double localSum2 = 0.0;
    for(i=0; i<n; i++)
    {
        localSum2 += ((arr[i] - globalMean1) * (arr[i] - globalMean1));
    }
    printf("\nlocalSum2 %16f", localSum2);
    double globalSum2;
    MPI_Reduce(&localSum2, &globalSum2, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    printf("\nglobalSum2 %16f", globalSum2);

    if(rank == 0)
    {
        double  res = 0.0;
        double globalMean2 = (double)globalSum2/(n *noPro);
        printf("\nglobalMean2 %16f " , globalMean2);
        res = sqrt(globalMean2);
        printf("\nResult : %16f ", res);
    }
    MPI_Finalize();
    return 0;
}
