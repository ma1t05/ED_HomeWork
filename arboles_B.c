
#include<stdlib.h>
#include<stdio.h>

#define TRUE 1
#define FALSE 0

struct B_tree{
  int t;
  int num_claves;
  int *claves;
  struct B_tree **nodos;
};

typedef struct B_tree arbol_B;

arbol_B* crea_arbol(int);
int busca(arbol_B*,int);

int main(){
  
  return 0;
}

arbol_B* crea_arbol(int t){
  int i;
  arbol_B *B;
  B = (arbol_B*)malloc(sizeof(arbol_B));
  B->t = t;
  B->claves = (int*)malloc(sizeof(int)*(2*t - 1));
  B->num_claves = 0;
  for(i = 0;i < 2*t - 1;i++){
    (B->claves)[i] = 0;
  }
  B->nodos = (arbol_B**)malloc(sizeof(arbol_B*)*(2*t));
  for(i = 0;i < 2*t;i++){
    (B->nodos)[i] = NULL;
  }
  return B;
}

int busca(arbol_B *B,int valor){
  int i;
  for(i = 0;i < B->num_claves - 1;i++){
    if((B->claves)[i] > valor){
      return busca((B->nodos)[i],valor);
    }
    else if ((B->claves)[i] == valor) {
      return TRUE;
    }
  }
  return busca((B->nodos)[B->num_claves],valor);
}

