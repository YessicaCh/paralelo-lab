#include <stdio.h>
#include <string.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>

double get_rank(int my_rank)
{
  srand(time(0));
  return rand()*my_rank;
}

int main(void){
  int my_rank, comm_sz;
  double sum;
  double sum_p;
  int divisor;

  MPI_Init(NULL,NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  sum=get_rank(my_rank);
  for(divisor=comm_sz/2;divisor>=1;divisor=divisor/2)
  {
    if(my_rank<2*divisor)
    {
      if(my_rank>=divisor)
      {
        MPI_Send(&sum,1,MPI_DOUBLE,my_rank-divisor,0,MPI_COMM_WORLD);

      }
      else{
        MPI_Recv(&sum_p,1,MPI_DOUBLE,my_rank+divisor,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        sum=sum+sum_p;
        
      }
    }
  }
  if (my_rank==0)
  {
    printf("La suma global es :%f\n",sum);
  }


  MPI_Finalize();
  return 0;
}