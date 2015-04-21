
#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#define TRUE 1
#define FALSE 0
#define MAX 50

struct arbol_binomico{
  int valor;
  //struct arbol_binomico *padre;
  struct arbol_binomico *hijo;
  struct arbol_binomico *hermano;
};

struct monticulo_binomico{
  int size;
  struct arbol_binomico ** tree;
};

typedef struct arbol_binomico tree;
typedef struct monticulo_binomico heap;

// 1.- Insertar, unir
void crece_monticulo(heap*);
tree* crea_arbol(int);
void agrega_valor(heap*,int);
// Imprimir
void heap_printf(heap*);
void tree_printf(tree*,int);
// 2.- Buscar
// 3.- Eliminar

int main(int argc, char **argv){
  heap *monticulo;
  int i,valor;
  
  //srand(time(NULL));
  monticulo = (heap*)malloc(sizeof(heap));
  monticulo->size = 0;
  monticulo->tree = NULL;

  // Agrega Valores
  printf("Agrega valores\n");
  for(i = 0;i < 15; i++){
    valor = rand()%MAX;
    printf("agrega: %d\n",valor);
    agrega_valor(monticulo,valor);
    //printf("size: %d\n",monticulo->size);
  }
  heap_printf(monticulo);
  /*
  avl_printf(arbol);
  preorden(arbol);
  inorder(arbol);
  postorden(arbol);
  */
  
}
/*
int busca_valor(avl **avl_tree,int valor){
  avl *tree;
  if(avl_tree != NULL && *avl_tree != NULL){
    tree = *avl_tree;
    if(!es_hoja(tree)){
      printf("[%d,%d]->",tree->valor,tree->profundidad);
      if(valor < tree->valor)
	return busca_valor(&(tree->izquierda),valor);
      else return busca_valor(&(tree->derecha),valor);
    }
    if(tree->valor == valor)
      return TRUE;
  }
  return FALSE;
}
*/
void crece_monticulo(heap *monticulo){
  int i;
  if(monticulo->size > 0){
    free(monticulo->tree);
  }
  monticulo->size +=1;
  monticulo->tree = (tree**)malloc(sizeof(tree*)*(monticulo->size));
  for(i = 0;i < monticulo->size;i++){
    (monticulo->tree)[i] = NULL;
  }
}

tree* crea_arbol(int valor){
  tree *arbol;
  arbol = (tree*)malloc(sizeof(tree));
  arbol->valor = valor;
  //arbol->padre = NULL;
  arbol->hijo = NULL;
  arbol->hermano = NULL;
  return arbol;
}

void agrega_valor(heap *monticulo,int valor){
  int k = 0;
  tree *arbol,*arbol_aux;
  arbol = crea_arbol(valor);

  while(k < monticulo->size){
    if((monticulo->tree)[k] == NULL){
      (monticulo->tree)[k] = arbol;
      return;
    }
    else {
      arbol_aux = (monticulo->tree)[k];
      if(arbol_aux->valor > arbol->valor){
	arbol->hermano = arbol_aux;
      }
      else {
	arbol_aux->hermano = arbol;
	arbol = arbol_aux;
      }
      (monticulo->tree)[k] = NULL;
    }
    k++;
  }
  
  crece_monticulo(monticulo);
  (monticulo->tree)[k] = arbol;
  
}

void heap_printf(heap *monticulo){
  tree **arbol = monticulo->tree;
  int k;
  for(k=0;k < monticulo->size; k++){
    printf("*****\nk=%d\n",k);
    if(arbol[k] != NULL){
      tree_printf(arbol[k],0);
    }
  }
}

void tree_printf(tree *arbol,int deep){
  int i;
  if(arbol == NULL) return;
  for(i=0;i<deep;i++)printf(" ");
  printf("%d\n",arbol->valor);
  tree_printf(arbol->hermano,++deep);
  tree_printf(arbol->hijo,deep);
}
/*
void preorden(avl **avl_tree){
  if(avl_tree == NULL || *avl_tree == NULL) return;
  printf("%d|",(*avl_tree)->valor);
  if(!es_hoja(*avl_tree)){
    preorden(&((*avl_tree)->izquierda));
    preorden(&((*avl_tree)->derecha));
  }
  if((*avl_tree)->padre == NULL)
    printf("\n");
}

void inorder(avl **avl_tree){
  if(avl_tree == NULL || *avl_tree == NULL) return;
  if(!es_hoja(*avl_tree))
    inorder(&((*avl_tree)->izquierda));
  printf("%d|",(*avl_tree)->valor);
  if(!es_hoja(*avl_tree)){
    inorder(&((*avl_tree)->derecha));
  }
  if((*avl_tree)->padre == NULL)
    printf("\n");
}

void postorden(avl **avl_tree){
  if(avl_tree == NULL || *avl_tree == NULL) return;
  if(!es_hoja(*avl_tree)){
    postorden(&((*avl_tree)->izquierda));
    postorden(&((*avl_tree)->derecha));
  }
  printf("%d|",(*avl_tree)->valor);
  if((*avl_tree)->padre == NULL)
    printf("\n");
}
*/
