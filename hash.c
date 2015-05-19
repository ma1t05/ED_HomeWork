
#include<stdlib.h>
#include<stdio.h>
#include "avl.h"
#define HASH_SIZE 16


struct hash_table{
  int size;
  int (*hash_function)(void*);
  avl *elementos;
};

typedef struct hash_table hash;

hash* hash_table_crea(int (*fh)(void*));
int hash_agrega_elemento(hash*,int,float);
float hash_busca_elemento(hash*,int);
int hash_elimina_elemento(hash*,int);

int fun_hash(void *i) {
  return (int)(*i) % HASH_SIZE;
}

int main(int argc,char **argv){
  hash *ht;
  ht = hash_table_crea((int (*)(int))fun_hash);
  free(ht);
  return 0;
}

hash* hash_table_crea(int (*funcion_hash)(int)){
  int i;
  hash* ht = NULL;
  ht = (hash*)malloc(sizeof(hash));
  ht->size = HASH_SIZE;
  ht->hash_function = funcion_hash;
  ht->elementos = (avl**)malloc(sizeof(avl*)*HASH_SIZE);
  for (i = 0;i < n;i++)
    (ht->elementos)[i] = NULL;
  return ht;
}

int hash_agrega_elemento(hash *ht,int clave,float valor){
  int aux;
  avl *t;
  aux = (*ht->hash_function)(clave);
  aux = aux % ht->size;
  if((ht->elementos)[aux] == NULL){
    t = avl_crea_hoja(NULL);
    t->clave = clave;
    t->valor = valor;
  }
  else {
    t = avl_agrega_valor(&((ht->elementos)[aux]),clave);
    if(t != NULL){
      t->valor = valor;
    }
    else {
      printf("Clave %d repetida, no fue posible agregar valor %f\n",clave,valor);
      return FALSE;
    }
  }
  return TRUE;
}

float hash_busca_elemento(hash *ht,int clave){
  int aux;
  avl *t;
  aux = (*ht->hash_function)(clave);
  aux = aux % ht->size;
  if((ht->elementos)[aux] == NULL){
    return -1.0;
  }
  else {
    t = avl_busca_valor(&(ht->elementos)[aux],clave);
    return (t != NULL ? t->valor : -1.0);
  }
}

int hash_elimina_elemento(hash *ht,int clave) {
  int aux;
  avl *t;
  aux = (*ht->hash_function)(clave);
  aux = aux % ht->size;
  if ((ht->elementos)[aux] == NULL) {
    return FALSE;
  }
  else {
    return avl_elimina_valor(&((ht->elementos)[aux]),clave);
  }    
}
