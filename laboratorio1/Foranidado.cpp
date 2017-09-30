#include<iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h> 
#include <stdio.h>


int main()
{
  int max=5000;
  int A[max][max];
  int x[max];
  int y[max];
srand (time(NULL));
 for(int i=0;i<max;i++)
    {for(int j=0;j<max;j++)
         {
	  A[i][j]=rand()%100;
         x[i]=rand();

	}
    }

for(int i=0;i<max;i++)
    {for(int j=0;j<max;j++)
         {
	  y[i]+=A[i][j]*x[j];
	}
    }

 return 1;
}
