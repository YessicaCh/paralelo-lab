#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <sys/time.h>


int main(int argc, char* argv[]){
  int thread_count =4;//strtol(argv[1], NULL, 10);
  int m,n,i,j;
  double *A,*x,*y;
  //double A[m][n],x[n],y[m];//**A, *x, *y;
  double elapsed;
  struct timeval begin, end;
  //scanf("%d %d", &m, &n);
  m=8000; 
  n=8000;
  A = malloc(m*n*sizeof(double));
  x = malloc(n*sizeof(double));
  y = malloc(m*sizeof(double));
   //double A[m][n],x[n],y[m];//**A, *x, *y;
  //valores de matriz 
  for(int h=0;h<m;h++){
    for(int g=0;g<n;g++){
         A[h*n+g]=drand48();
    }
  }
  //valores para el vector 
  for(j = 0; j < n;j++){
      x[j] = drand48();
    }
  elapsed=0.0;
 for(int p=0;p<20;p++){
  gettimeofday(&begin, NULL);
#pragma omp parallel for num_threads(thread_count) default(none) private(i,j) shared(A,x,y,m,n)
  for(i = 0; i < m; i++){
        y[i] = 0.0;
        for(j = 0; j < n; j++)
          y[i] +=  A[i*n+j] * x[j];
      }
  gettimeofday(&end, NULL);
  elapsed = elapsed+((end.tv_sec - begin.tv_sec) * 1000000u + end.tv_usec - begin.tv_usec) / 1.e6;
  printf("%lf\n",((end.tv_sec - begin.tv_sec) * 1000000u + end.tv_usec - begin.tv_usec) / 1.e6);
  }
  //elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
  printf("\n");
  printf("%lf\n", elapsed/20);
  return 0;
}