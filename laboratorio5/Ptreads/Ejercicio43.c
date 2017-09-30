#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_THREAD 4

double  a, b, h;
int     n, local_n;
double  total;// variable global en conflicto

//para busy_waiting
int flag=0;
//PARA MUTEX
pthread_mutex_t   mutex;
//para semaforo
sem_t   sem;

void *Thread_Busy_waiting(void* rank);
void *Thread_mutex(void* rank);
void *Thread_semaforo(void* rank);
double Trap(double local_a, double local_b, int local_n,double h);    /* Calcula la integral local */
double f(double x); 

int main(int argc, char** argv) {
    long        i;
   pthread_t  threaD[NUM_THREAD];  

    total = 0.0;
   
    printf("Enter a, b, n\n");
    scanf("%lf %lf %d", &a, &b, &n);
    h = (b-a)/n;
    local_n = n/NUM_THREAD;

    for (i = 0; i < NUM_THREAD; i++) {
         pthread_create(&threaD[i], NULL, Thread_mutex, 
               (void*) i);
    }

    for (i = 0; i < NUM_THREAD; i++) {
        pthread_join(threaD[i], NULL);
    }

    printf("Con n = %d \n",n);
    printf("halla la integral desde %f hasta %f = %19.15e\n", a, b, total);

    return 0;
}
void *Thread_Busy_waiting(void* rank) {
    double      local_a;   
    double      local_b;   
    double      my_int;  
    long        my_rank = (long) rank;

    local_a = a + my_rank*local_n*h;
    local_b = local_a + local_n*h;

    my_int = Trap(local_a, local_b, local_n, h);

    while (flag!=my_rank);
     total += my_int;
        flag=(flag+1)%NUM_THREAD;
    
  
    return NULL;
}


void *Thread_mutex(void* rank) {
    double      local_a;   
    double      local_b;   
    double      my_int;  
    long        my_rank = (long) rank;

    local_a = a + my_rank*local_n*h;
    local_b = local_a + local_n*h;

    my_int = Trap(local_a, local_b, local_n, h);

    pthread_mutex_lock(&mutex);
    total += my_int;
    pthread_mutex_unlock(&mutex);

    return NULL;

}  
void *Thread_semaforo(void* rank) {
    double      local_a;   
    double      local_b;   
    double      my_int;  
    long        my_rank = (long) rank;

    local_a = a + my_rank*local_n*h;
    local_b = local_a + local_n*h;

    my_int = Trap(local_a, local_b, local_n, h);

    sem_wait(&sem);
    total += my_int;
    sem_post(&sem);
    return NULL;
}

double Trap(double local_a,double local_b ,int local_n,double  h) {

    double integral;   //rpt;
    double x;
    int i;

    integral = (f(local_a) + f(local_b))/2.0;
    x = local_a;
    for (i = 1; i <= local_n-1; i++) {
        x = local_a + i*h;
        integral += f(x);
    }
    integral = integral*h;
    return integral;
} 

double f(double x) {
    double return_val;

    return_val = x*x;
    return return_val;
} 