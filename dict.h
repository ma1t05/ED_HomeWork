
#ifndef DICT_H
#define DICT_H

#include<stdlib.h>

#define TRUE 1
#define FALSE 0

struct avl_tree {
  int key;
  void *value;
  struct avl_tree *left,*right,*p;
};

struct dicctionary {
  int elements;
  struct avl_tree **root;
};

typedef struct avl_tree avl;
typedef struct dicctionary dict;

dict* crea_dict();
int agrega_clave(dict*,int,void*);
int es_elemento(dict*,int);
avl* busca_clave(dict*,int);
void* elimina_clave(dict*,int);

int es_hoja(avl*);
int _es_elemento(avl*,int);
avl* _busca_clave(avl*,int);
avl* _agrega_clave(avl**,int);
avl* avl_crea_hoja(avl*);

#endif
