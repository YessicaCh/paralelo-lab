#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <sys/time.h>

// PARA COMPILAR
//gcc -g -Wall -fopenmp -o a oddEvenSort.c
//  ./a


int main(int argc, char const *argv[])
{


double factor = 1.0,pi_approx;
double sum = 0.0;
int thread_count=2,k,n=1000;

#pragma omp parallel for num_threads(thread_count) \
default(none) reduction(+:sum) private(k, factor) \
shared(n)
for (k = 0; k < n; k++) {

		if (k % 2 == 0)
		  factor = 1.0;
		else
		  factor = - 1.0;

		sum = sum + factor/(2*k+1);
		printf("sum %f\n",sum);

		}
 pi_approx = 4.0*sum;

  printf("\n");
  printf("sum %f\n",pi_approx);



}

/*#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
void hello(void);
void estimacion_pi();
int main()
{
	
    int thread_count=4;
    #pragma omp parallel num_threads(thread_count)
    hello();

	return 0;
}

void hello(void)
 {
	int my_rank=omp_get_thread_num();
	int thread_count=omp_get_num_threads();
	printf("hello from thread %d of %d\n",my_rank,thread_count);
}

void estimacion_pi(){

double factor = 1.0;
double sum = 0.0;
# pragma omp parallel for num threads(thread count) \
   reduction(+:sum)
for (k = 0; k < n; k++) {
	if (k%2==0)
		factor=1.0;
	else 
		factor=-1.0;
	sum += factor/(2∗k+1);
	//factor = − factor;
}
pi approx = 4.0∗sum;

}*/
