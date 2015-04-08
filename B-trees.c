
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
  struct B_node **root;
};

typedef struct B_node B_node;
typedef struct B_tree B_tree;

int b_tree_search(B_node *x,key k){
  int i = 0;
  while(i <= x->n && k > x->key[i]) i++;
  if (i <= x->n && k == x->key[i])
    return TRUE;
  if (x->leaf)
    return FALSE;
  return b_tree_search(x->c[i],k);
}

B_tree* B_tree_create(){
  B_tree *T;
  B_node *x;
  x = (B_node*)malloc(sizeof(B_node));
  x->leaf = TRUE;
  x->n = 0;
  T = (B_tree*)malloc(sizeof(B_tree));
  T->root = x;
  return T;
}

void B_tree_split_child(B_node *x,int i){
  B_node *y,*z;
  z = (B_node*)malloc(sizeof(B_node));
  y = x->c[i];
  z->leaf = y->leaf;
  for(j = 0;j < B_TREE_T - 1;j++)
    z->key[j] = y->key[j+B_TREE_T];
  if (!y->leaf) {
    for(j = 0;j < B_TREE_T - 1;j++)
      z->c[j] = y->c[j+B_TREE_T];
  }
}
