
#include<stdlib.h>
#include<stdio.h>

#define HASH_SIZE 16


struct AVL_tree{
  int altura, profundidad;
  int claves;
  int clave;
  float valor;
  struct AVL_tree *izquierda;
  struct AVL_tree *derecha;
  struct AVL_tree *padre;
};

typedef struct AVL_tree avl;

int busca_valor(avl**,int);
int agrega_valor(avl**,int,float);
int elimina_valor(avl**,int);
void avl_free(avl**);
int es_hoja(avl*);
void cheka_balance(avl**);

avl* avl_crea_hoja(avl*);
void actualiza_altura(avl*);
int actualiza_profundidad(avl*);
void actualiza_claves(avl*);

struct hash_table{
  int size;
  int (*hash_function)(int);
  avl **elementos;
};

typedef struct hash_table hash;

hash* crea_hash_table(int (*fh)(int));
int agrega_elemento(hash*,int,float);
float busca_elemento(hash*,int);
int elimina_elemento(hash*,int);

int fun_hash(int i){
  return i % HASH_SIZE;
}

int main(int argc,char **argv){
  hash * ht = NULL;
  ht = crea_hash_table((int (*)(int))fun_hash);
  return 0;
}

hash* crea_hash_table(int (*funcion_hash)(int)){
  int i;
  hash* ht = NULL;
  ht = (hash*)malloc(sizeof(hash));
  ht->size = HASH_SIZE;
  ht->hash_function = funcion_hash;
  ht->elementos = (avl**)malloc(sizeof(avl*)*HASH_SIZE);
  for(i = 0; i < HASH_SIZE;i++){
    (ht->elementos)[i] = NULL;
  }
  return ht;
}

int agrega_elemento(hash *ht,int clave,float valor){
  int aux;
  avl *t;
  aux = (*ht->hash_function)(clave);
  if((ht->elementos)[aux] == NULL){
    t = avl_crea_hoja(NULL);
    t->clave = clave;
    t->valor = valor;
  }
  else {
    t = agrega_valor(&(ht->elementos)[aux],clave);
    if(t != NULL){
      t->valor = valor;
    }
    else {
      printf("Clave %d repetida, no fue posible agregar valor %f\n",clave,valor);
    }
  }
}

float busca_elemento(hash *ht,int clave){
  int aux;
  avl *t;
  aux = (*ht->hash_function)(clave);
  if((ht->elementos)[aux] == NULL){
    return -1.0;
  }
  else {
    t = busca_valor(&(ht->elementos)[aux],clave);
    return (t != NULL ? t->valor : -1.0);
  }
}
