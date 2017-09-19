#include <stdio.h>
#include <string.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>

int main(void){
  int my_rank, comm_sz,toss,number_of_tosses;
  //double a=0.0, b=3.0, h,local_a, local_b;
  double number_in_circle,pi_estimate,distance_squared;
  double x,y;
  int source;

  MPI_Init(NULL,NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

  if (my_rank==0)
  {
  	printf("Ingrese numero de Tosess \n");
	scanf("%d",&number_of_tosses);
  }
  MPI_Bcast(&number_of_tosses,1,MPI_INT,0,MPI_COMM_WORLD);// envia  n a todos los otros procesos
  if (number_of_tosses<=0)
  {
  	 MPI_Finalize();

  }

  else
  {

  number_in_circle=0;
  for(toss=0;toss<number_of_tosses;toss++)
   {
   	 srand(time(0));
   	 x=rand()%2;
   	 y=rand()%2;
   	 distance_squared=x*x+y*y;
   	 if(distance_squared<=1)
   	 	number_in_circle++;
   }
   pi_estimate=4*number_in_circle/((double)number_of_tosses);
   MPI_Reduce(&number_in_circle,&pi_estimate,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
   }
   if(my_rank==0)
   {
   	 pi_estimate=4*number_in_circle/((double)number_of_tosses);
   	printf("La estimacion de PI es :%f\n",pi_estimate);
   }
  //Get_input(my_rank,comm_sz,&local_a,&local_b,&local_n);
  //h=(b-a)/n;
  //local_n = n/comm_sz;

  //local_a = a + my_rank*local_n*h;
  //local_b = local_a + local_n*h;
  //local_int = Trap(local_a, local_b, local_n, h);
  //MPI_Reduce(&local_int, &total_int, 1, MPI_DOUBLE, MPI_SUM, 0,MPI_COMM_WORLD);

 /* if(my_rank != 0){
    MPI_Send(&local_int, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
  }
  else{
    total_int = local_int;
    for(source = 1; source < comm_sz; source++){
      MPI_Recv(&local_int,1,MPI_DOUBLE,source,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
      total_int += local_int;
   } 
  }*/

 /* if(my_rank == 0){
    printf("With n = %d trapezoids, our estimate\n", n);
    printf("of the integral from %f to %f = %.15e\n", a, b, total_int);
  }*/
  MPI_Finalize();
  return 0;
}