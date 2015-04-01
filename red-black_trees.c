
/*
 * Autor: Luis Maltos
 * Clase: Estructura de Datos en C++
 * Implementacion de arboles rojo-negro
 * Se imprementa:
 *  buscar, predecesor, sucesor, minimo, maximo,
 *  insertar, eliminar
 * Fecha: 31 de Marzo de 2015
 */

#include<stdlib.h>

#define TRUE 1
#define FALSE 0
#define red 'r'
#define black 'b'

struct node{
  char color;
  int key;
  struct node *left,*right,*p;
};

typedef struct node node;

node* _rbt_search(node *T,int key){
  if(T == NULL) return NULL;
  if(T->key == key)
    return T;
  else if(key < T->key)
    return _rbt_search(T->left,key);
  else
    return _rbt_search(T->right,key);
}

int rbt_search(node *T,int key){
  node *n = _rbt_search(T,key);
  if(n != NULL)
    return TRUE;
  else return FALSE;
}

int rbt_predecessor(node *T,int key){
  int predecessor;
  if(T == NULL) return key;
  if (T->key >= key){
    return rbt_predecessor(T->left,key);
  }
  else {
    predecessor = rbt_predecessor(T->right,key);
    return (predecessor < key ? predecessor : T->key);
  }
}

int rbt_successor(node *T,int key){
  int successor;
  if(T == NULL) return key;
  if(T->key >= key){
    successor = rbt_successor(T->left,key);
    return (successor > key ? successor : T->key);
  }
  else {
    return rbt_successor(T->right,key);
  }
}

int rbt_minimum(node *T){
  if(T->left != NULL)
    return rbt_minimum(T->left);
  return T->key;
}

int rbt_maximum(node *T){
  if(T->right != NULL)
    return rbt_maximum(T->right);
  return T->key;
}

int rbt_insert(node **T,int key){
  return FALSE;
}

int rbt_delete(node **T,int key){
  return FALSE;
}

int main(int argc,char *argv[]){
  
  return 0;
}
