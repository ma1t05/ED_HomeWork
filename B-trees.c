
/*
 * Author: Luis Maltos
 * Clase: Estructura de Datos en C++
 * Implementacion de arboles B
 * Se implementa:
 *  search, create, insert
 */

#include<stdlib.h>

#define TRUE 1
#define FALSE 0
#define B_TREE_T 3

struct B_node{
  int n;
  int *key;
  int leaf;
  struct B_node **c;
};

struct B_tree{
  struct B_node *root;
};

typedef struct B_node B_node;
typedef struct B_tree B_tree;

int b_tree_search(B_node *x,int k){
  int i = 0;
  while(i <= x->n && k > x->key[i]) i++;
  if (i <= x->n && k == x->key[i])
    return TRUE;
  if (x->leaf)
    return FALSE;
  return b_tree_search(x->c[i],k);
}

B_node* B_node_create(){
  int i;
  B_node *x;
  x = (B_node*)malloc(sizeof(B_node));
  x->n = 0;
  x->leaf = TRUE;
  x->key = (int*)malloc(sizeof(int)*(2*B_TREE_T - 1));
  x->c = (B_node**)malloc(sizeof(B_node*)*(2*B_TREE_T));
  for(i = 0;i < 2*B_TREE_T;i++) x->c[i] = NULL;
}

B_tree* B_tree_create(){
  B_tree *T;
  B_node *x;
  x = B_node_create();
  T = (B_tree*)malloc(sizeof(B_tree));
  T->root = x;
  return T;
}

void B_tree_split_child(B_node *x,int i){
  int j;
  B_node *y,*z;
  z = B_node_create();
  y = x->c[i];
  z->leaf = y->leaf;
  z->n = B_TREE_T - 1;
  for(j = 0;j < B_TREE_T - 1;j++)
    z->key[j] = y->key[j+B_TREE_T];
  if (!y->leaf) {
    for(j = 0;j < B_TREE_T;j++)
      z->c[j] = y->c[j+B_TREE_T];
  }
  y->n = B_TREE_T - 1;
  for(j = x->n + 1;j > i;j--)
    x->c[j] = x->c[j-1];
  x->c[i] = z;
  for(j = x->n;j > i;j--)
    x->key[j] = x->key[j-1];
  x->key[i] = y->key[B_TREE_T - 1];
  x->n++;
}

void B_tree_insert_nonfull(B_node *x,int key){
  int i = x->n;
  if (x->leaf) {
    while (i > 0 && k < x->key[i-1])
      x->key[i] = x->key[--i]; // Precaucion!, validar orden de aplicacion
    x->key[i] = key;
    x->n++;
  }
  else {
    while (i > 0 && k < x->key[i-1])
      i--;
    if (x->c[i]->n == 2 * B_TREE_T - 1) {
      B_tree_split_child(x,i);
      if (k > x->key[i]) i++;
    }
    B_tree_insert_nonfull(x->c[i],k);
  }
}

void B_tree_insert(B_tree *T,int key){
  B_node *r;
  r = T->root;
  if (r->n == 2 * B_TREE_T - 1) {
    s = B_node_create();
    T->root = s;
    s->leaf = FALSE;
    s->n = 0;
    s->c[0] = r;
    B_tree_split_child(s,1);
    r = s;
  }
  B_tree_insert_nonfull(r,key);
}
