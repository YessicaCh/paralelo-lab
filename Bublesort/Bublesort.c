#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

/* the number of data elements in each process */
#define N 16384


/* initialize the data to random values based on rank (so they're different) */
void init(int* data, int rank) {
  int i;
  srand(rank);
  for (i = 0; i < N; i++) {
    data[i] = rand( ) % 100;
  }
}

/* print the data to the screen */
void print(int* data, int rank) {
  int i;
  printf("Process: ");
  for (i = 0; i < N; i++) {
    printf("%d ", data[i]);
  }
  printf("\n");
}

/* comparison function for qsort */
int cmp(const void* ap, const void* bp) {
  int a = * ((const int*) ap);
  int b = * ((const int*) bp);

  if (a < b) {
    return -1;
  } else if (a > b) {
    return 1;
  } else {
    return 0;
  }
}

/* find the index of the largest item in an array */
int max_index(int* data) {
  int i, max = data[0], maxi = 0;

  for (i = 1; i < N; i++) {
    if (data[i] > max) {
      max = data[i];
      maxi = i;
    }
  }
  return maxi;
}

/* find the index of the smallest item in an array */
int min_index(int* data) {
  int i, min = data[0], mini = 0;

  for (i = 1; i < N; i++) {
    if (data[i] < min) {
      min = data[i];
      mini = i;
    }
  }
  return mini;
}


/* do the parallel odd/even sort */
void parallel_sort(int* data, int rank, int size) {
  int i;

  /* the array we use for reading from partner */
  int other[N];

  /* we need to apply P phases where P is the number of processes */
  for (i = 0; i < size; i++) {
    /* sort our local array */
    qsort(data, N, sizeof(int), &cmp);

    /* find our partner on this phase */
    int partener;

    /* if it's an even phase */
    if (i % 2 == 0) {
      /* if we are an even process */
      if (rank % 2 == 0) {
        partener = rank + 1;
      } else {
        partener = rank - 1;
      }
    } else {
      /* it's an odd phase - do the opposite */
      if (rank % 2 == 0) {
        partener = rank - 1;
      } else {
        partener = rank + 1;
      }
    }

    /* if the partener is invalid, we should simply move on to the next iteration */
    if (partener < 0 || partener >= size) {
      continue;
    }

    /* do the exchange - even processes send first and odd processes receive first
     * this avoids possible deadlock of two processes working together both sending */
    if (rank % 2 == 0) {
      MPI_Send(data, N, MPI_INT, partener, 0, MPI_COMM_WORLD);
      MPI_Recv(other, N, MPI_INT, partener, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else {
      MPI_Recv(other, N, MPI_INT, partener, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Send(data, N, MPI_INT, partener, 0, MPI_COMM_WORLD);
    }

    /* now we need to merge data and other based on if we want smaller or larger ones */
    if (rank < partener) {
      /* keep smaller keys */
      while (1) {
        /* find the smallest one in the other array */
        int mini = min_index(other);

        /* find the largest one in out array */
        int maxi = max_index(data);

        /* if the smallest one in the other array is less than the largest in ours, swap them */
        if (other[mini] < data[maxi]) {
          int temp = other[mini];
          other[mini] = data[maxi];
          data[maxi] = temp;
        } else {
          /* else stop because the smallest are now in data */
          break;
        }
      }
    } else {
      /* keep larger keys */
      while (1) {
        /* find the largest one in the other array */
        int maxi = max_index(other);

        /* find the largest one in out array */
        int mini = min_index(data);

        /* if the largest one in the other array is bigger than the smallest in ours, swap them */
        if (other[maxi] > data[mini]) {
          int temp = other[maxi];
          other[maxi] = data[mini];
          data[mini] = temp;
        } else {
          /* else stop because the largest are now in data */
          break;
        }
      }
    }
  }
}
int main(void) {
  /* our rank and size */
  int rank, size;
  double MPI_Wtime(void);
  double start,finish;
  start=MPI_Wtime(); 	
  /* our processes data */
  int data[N];

  /* initialize MPI */
  MPI_Init(NULL, NULL);

  /* get the rank (process id) and size (number of processes) */
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  /* initialize the data */
  init(data, rank);

  /* do the parallel odd/even sort */
  parallel_sort(data, rank, size);

  /* now print  our data */
  //print(data, rank);

  /* quit MPI */
  finish=MPI_Wtime();
  printf("proc %d > Elapsed time = %e seconds\n",rank,(finish-start));
  MPI_Finalize( );
  return 0;
}

/*
Con 100 datos y 8 procesos:
 
proc 0 > Elapsed time = 5.506039e-03 seconds = 0.006 seconds
proc 1 > Elapsed time = 5.243063e-03 seconds = 0.005 seconds
proc 2 > Elapsed time = 4.382133e-03 seconds = 0.004 seconds
proc 3 > Elapsed time = 4.403114e-03 seconds = 0.004 seconds
proc 4 > Elapsed time = 4.300117e-03 seconds = 0.004 seconds
proc 5 > Elapsed time = 3.488064e-03 seconds = 0.003 seconds
proc 6 > Elapsed time = 3.525019e-03 seconds = 0.003 seconds
proc 7 > Elapsed time = 2.791166e-03 seconds = 0.002 seconds
*/

/*
Con 500 datos y 8 procesos
proc 0 > Elapsed time = 1.160002e-02 seconds = 0.012 seconds
proc 1 > Elapsed time = 1.140285e-02 seconds = 0.011 seconds
proc 2 > Elapsed time = 1.078796e-02 seconds = 0.010 seconds
proc 3 > Elapsed time = 1.006889e-02 seconds = 0.012 seconds
proc 4 > Elapsed time = 1.033902e-02 seconds = 0.010 seconds
proc 5 > Elapsed time = 1.028514e-02 seconds = 0.010 seconds
proc 6 > Elapsed time = 9.727001e-03 seconds = 0.009 seconds
proc 7 > Elapsed time = 9.760141e-03 seconds = 0.009 second
*/

/*
Con 10000 datos y 8 procesos:
proc 0 > Elapsed time = 2.605651e+00 seconds
proc 1 > Elapsed time = 2.601222e+00 seconds
proc 2 > Elapsed time = 2.600468e+00 seconds
proc 3 > Elapsed time = 2.504631e+00 seconds
proc 4 > Elapsed time = 2.503775e+00 seconds
proc 5 > Elapsed time = 2.614560e+00 seconds
proc 6 > Elapsed time = 2.613675e+00 seconds
proc 7 > Elapsed time = 2.616782e+00 seconds
*/
/*#include<stdio>

void Merge low(int my_keys[],int recv_keys[],int temp_keys[],int local_n int m_i, r_i, t_i;
{
m_i = r_i = t_i = 0;
while (t_i < local_n) {
	if (my_keys[m_i] <= recv_keys[r_i]) {
		temp_keys[t_i] = my_keys[m_i];
		t_i++; m_i++;
	} 
	else {
	temp_keys[t_i] = recv_keys[r_i];
	t_i++; r_i++;
	     }
	}

for (m_i = 0; m_i < local_n; m_i++)
my_keys[m_i] = temp_keys[m_i];
/∗ Merge low ∗/
}
int main()
{
	if (phase % 2 == 0)
		if (my rank % 2 != 0) /∗ Odd rank ∗/
		    partner = my rank − 1;
		else/∗ Even rank ∗/
		    partner = my rank + 1;
	else/∗ Odd phase ∗/
		if (my rank % 2 != 0)/∗ Odd rank ∗/
		   partner = my rank + 1;
		else/∗ Even rank ∗/
		   partner = my rank − 1;
	if (partner == − 1 | | partner == comm sz)
	partner = MPI PROC NULL;
    if (my rank % 2 == 0) {
			MPI_Send(msg, size, MPI INT, (my rank+1) % comm sz, 0,
			MPI_Recv(new msg, size, MPI INT, (my rank+comm sz − 1) %
			0, comm, MPI STATUS IGNORE);} 
else {
			MPI Recv(new msg, size, MPI INT, (my rank+comm sz − 1) %
			0, comm, MPI STATUS IGNORE).
			MPI Send(msg, size, MPI INT, (my rank+1) % comm sz, 0,
			}
return 0;
}*/