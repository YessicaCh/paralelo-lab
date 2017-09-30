
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define NUM_THREADS 4
#define TOSSES   1205
int number_in_circle[NUM_THREADS];
/*RUN:gcc -g -wall -o pthread Ejercicico42.c -pthread*/
void *monteCarlo(void *threadid)
{
  //double number_in_circle;
   int tid;
   tid = (long)threadid; // el thead 
   int toss;
  double distance_squared;
   double tosses_local=TOSSES/NUM_THREADS;
   number_in_circle[tid]=0;
   double x,y;
   for(toss=0;toss<tosses_local;toss++)
   { srand(time(0));

      x=drand48();//rand()%1;//drand48();
      y=drand48();//rand()%1;
      distance_squared=x*x+y*y;
      if(distance_squared<=1)number_in_circle[tid]++;
   }
  // pi_estimate=4*number_in_circle/((double)tosses);
   //printf("Hello World! It's me, thread #%ld!\n", tid);
   pthread_exit(NULL);
}

int main(int argc, char *argv[])
{

   pthread_t threads[NUM_THREADS];
  // int rc;
   long t;
   int totalnumber_in_circle;
   double pi=0;
   //long long toss;
   /*printf("Ingrese numero de tosses");
   scanf(toss);*/
   for(t=0;t<NUM_THREADS;t++){
     pthread_create(&threads[t], NULL,monteCarlo, (void *)t);
     }
     for(t=0;t<NUM_THREADS;t++)
     {
        pthread_join(threads[t],NULL);
     }
     totalnumber_in_circle=0;
     for(int i=0;i<NUM_THREADS;i++)
        {
        totalnumber_in_circle=totalnumber_in_circle+number_in_circle[i];
        }
     pi=4.0*(double)totalnumber_in_circle/(double)TOSSES;

     printf("La estimacion de PI is: %f\n", pi);
   pthread_exit(NULL);
}