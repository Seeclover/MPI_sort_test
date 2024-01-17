#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int n;
int A[70002];

int com(const void *x, const void *y)
{
    return (*(int*)x - *(int*)y);
}

int main(int argc, char *argv[])
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    if (rank == 0)
    {
        char file_name[10000];
        scanf("%s", file_name);
        if (file_name == NULL)
        {
            MPI_Finalize();
            return 0;
        }
        FILE *file = fopen(file_name, "r");
        fscanf(file, "%d", &n);
        for (int i = 0; i < n; i++)
        {
            fscanf(file, "%d", &A[i]);
        }
    }
    
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
        
    int n1 = n;
    if (n % size != 0)
    {
        n1 += (size - (n % size));
    }
    int subA[n1 / size];
    n1 /= size;
    MPI_Scatter(A, n1, MPI_INT, subA, n1, MPI_INT, 0, MPI_COMM_WORLD);
    
    if (rank == size - 1)
    {
        if (n % size != 0)
        {
            n1 -= (size - (n % size));
        }
    }

    qsort(subA, n1, sizeof(int), com);

    if (rank == size - 1)
    {
        if (n % size != 0)
        {
            n1 += (size - (n % size));
        }
    }
    
    
    MPI_Gather(subA, n1, MPI_INT, A, n1, MPI_INT, 0, MPI_COMM_WORLD);
        
    if (rank == 0)
    {
        int now[size][2];
        int a = 1000000;
        int ans[n];
        for (int i = 0; i < size; i++)
        {
            now[i][0] = A[n1 * i];
            now[i][1] = n1 * i;
        }

        for (int i = 0; i < n; i++)
        {
            a = 1000000;
            int kk;
            for (int i2 = 0; i2 < size; i2++)
            {
                if (now[i2][0] < a)
                {
                    a = now[i2][0];
                    kk = i2;
                }
            }
            ans[i] = a;
            if ((now[kk][1] + 1) % n1 != 0)
            {
                now[kk][1]++;
                now[kk][0] = A[now[kk][1]];
            }
            else
            {
                now[kk][0] = 1000000;
            }
        }

        for (int i = 0; i < n; i++)
        {
            printf("%d ", ans[i]);
        }
    }
    
    MPI_Finalize();
}
