/* 
   Estructura de Datos en C++
   Tarea 1
   21 de Enero de 2015
   Luis Maltos
*/

#include<stdio.h>
#include<stdlib.h>

int unif(int);
int cmpr(const void*,const void*);
int search(int *,unsigned int,int);
int sort_search(int *,unsigned int,int);
int binary_search(int *,unsigned int,unsigned int,int);
void swap(int *,int *);
void permute(int *,int,int);

unsigned int m;
int c_u,permutations;

int main(){
  unsigned int n;
  int i,j;
  int *a,*A,*M,x;
  int c_s,c_b;
  
  printf("m\tn\tunsort\tsort\tbinary\n");
  for(m = 100;m <= 500;m+=100){
    M = (int*) malloc(m*sizeof(int));
    for(n = 2;n <= 10;n++){
      // asignar memoria
      a = (int*) malloc(n*sizeof(int));
      A = (int*) malloc(n*sizeof(int));

      // Llena arreglo a sin repeticiones
      for(i = 0;i < m;i++) M[i]=0;
      i = 0;
      do{
	x = unif(m);
	if(M[x] == 0){
	  M[x] = 1;
	  a[i++] = x;
	}
      } while(i < n);
      
      // Ordenar arreglo
      qsort(a,n,sizeof(int),cmpr);
      
      for(i = 0;i < n;i++) A[i] = a[i];
      
      c_u = 0;
      permutations = 0;
      permute(a,0,n-1);
      c_s = 0;
      c_b = 0;
      for(i = 1;i <= m;i++){
	c_s += sort_search(A,n,i);
	c_b += binary_search(A,0,n-1,i);
      }
      
      printf("%d\t%d\t%d\t%d\t%d\n",m,n,c_u/permutations,c_s,c_b);

      // liberar memoria
      free(a);
      free(A);
    }
    free(M);
  }
  return 0;
}

int unif(int n){
  return rand() % n + 1;
}

int cmpr(const void* a,const void* b){
  return (*(int*)a - *(int*)b);
}

int search(int *a,unsigned int size,int j){
  int i,comparations = 0;
  for(i = 0;i < size;i++){
    comparations++;
    if(a[i] == j) break;
  }
  return comparations;
}

int sort_search(int *a,unsigned int size, int j){
  int i,comparations = 0;
  for(i = 0;i < size;i++){
    comparations++;
    if(a[i] >= j) break;
  }
  return comparations;
}

int binary_search(int *a,unsigned int i,unsigned int j,int k){
  if(i >= j - 1) return 1;
  if(a[(i + j)/2] > k)
    return 1 + binary_search(a,(i + j)/2,j,k);
  if(a[(i + j)/2] < k)
    return 1 + binary_search(a,i,(i + j)/2,k);
  return 1;	  
}

void swap(int *a, int *b){
  int x = *a;
  *a = *b;
  *b = x;
}

void permute(int *a, int i, int j){
  int k;
  if(i == j) { // Se tiene una permutacion
    for(k = 1;k <= m;k++)
      c_u += search(a,j,k);
    permutations++;
  }
  else {
    for(k = i;k <= j;k++){
      swap(a+i,a+j);
      permute(a,i+1,j);
      swap(a+1,a+j);
    }
  }
}
