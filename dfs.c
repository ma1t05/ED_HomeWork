
#include<stdlib.h>
#include<stdio.h>

#define TRUE 1
#define FALSE 0

struct vertice{
  char etiqueta;
  int marcado;
  int n_vecinos;
  struct vertice *vecinos;
};

struct vertices{
  int num_elem;
  struct vertice *nodos;
};

typedef struct vertice nodo;
typedef struct vertices node_set;

int main(int argc,char **argv){
  char aristas[][3] = {"ad","ac","ae","bc","be","bf","de","fc"};
  char inicio,final;
  node_set *Nodos;
  int i;

  Nodos = (node_set*)malloc(sizeof(node_set));
  Nodos->num_elem = 0;
  Nodos->nodos  = NULL;
  
  for(i = 0;i < 8;i++){
    inicio = aristas[i][0];
    final = aristas[i][1];
    
  }
  return 0;
}

int es_elemento(node_set *Nodos,char key){
  
}
