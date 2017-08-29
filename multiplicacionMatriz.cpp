#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h> 
 int max=10;
void llenar_matriz( int **matriz)
{matriz= new int*[max];cd 
 for(int i=0;i<max;i++)
	{ matriz[i]=new int[max];
	}
for(int i=0;i<max;i++)
    {for(int j=0;j<max;j++)
         {
	  matriz[i][j]=rand()%100;F
       
	}
    }
  
}

void multiplicar_matriz_clasico(int **A,int **B)
{ int **C;
  C= new int*[max];
 for(int i=0;i<max;i++)
	{ C[i]=new int[max];
	}

// multiplicacion
for(int i=0;i<max;i++)
    {for(int j=0;j<max;j++)
         {
	  C[i][j]=0;
          for(int k=0;k<max;k++)
		{C[i][j]=C[i][j]+A[i][k]*B[k][j];}
         }
    }
}

int main()
{
  int **A;
  int **B;
  llenar_matriz(A);
  llenar_matriz(B);
  multiplicar_matriz_clasico(A,B);
 return 0;
}
