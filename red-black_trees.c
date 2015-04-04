
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
#include<stdio.h>

#define TRUE 1
#define FALSE 0
#define RED 'r'
#define BLACK 'b'
#define MAX 100

struct node{
  char color;
  int key;
  struct node *left,*right,*p;
};

typedef struct node node;

void _rbt_insert_fixup(node*);
void _rbt_left_rotate(node**);
void _rbt_right_rotate(node**);
void _rbt_transplant(node**,node*);

node* rbt_node(node *p,int key){
  node *x;
  x = (node*)malloc(sizeof(node));
  x->color = RED;
  x->key = key;
  x->left = NULL;
  x->right = NULL;
  x->p = p;
  return x;
}

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
  node *x,*y,*z;
  y = NULL;
  x = *T;
  while(x != NULL){
    printf("Pasa nodo %d\n",x->key);
    y = x;
    if(key < x->key)
      x = x->left;
    else x = x->right;
  }
  z = rbt_node(y,key);
  if(y == NULL){
    printf("Agrega raiz\n");
    *T = z;
  }
  else if (key < y->key) {
    printf("Agrega a la izquierda\n");
    y->left = z;
  }
  else { // Validar caso cuando ya se tiene la etiqueta
    printf("Agrega a la derecha\n");
    y->right = z;
  }
  printf("Comienza reparacion\n");
  _rbt_insert_fixup(z);
}

int rbt_delete(node **T,int key){
  char color;
  node *x,*y,*z = _rbt_search(*T,key);
  if (z == NULL) return FALSE;
  y = z;
  color = y->color;
  if (z->left == NULL) {
    x = z->right;
    _rbt_transplant(&z,z->right);
  }
  else if (z->right == NULL) {
    x = z->left;
    _rbt_transplant(&z,z->left);
  }
  else {
    y = _rbt_search(z->right,rbt_minimum(z->right));
    color = y->color;
    x = y->right;
    if(y->p == z)
      x->p = y;
    else {
      _rbt_transplant(&y,y->right);
      y->right = z->right;
      y->right->p = y;
    }
    _rbt_transplant(&z,y);
    y->left = z->left;
    y->left->p = y;
    y->color = z->color;
  }
  if (color == BLACK) {
    // Fix Up
  }
  return FALSE;
}

int main(int argc,char *argv[]){
  int i,v;
  node **T;
  T = (node**)malloc(sizeof(node*));
  *T = NULL;

  for(i = 0;i < 10;i++){
    v = rand() % MAX;
    printf("Inserta %d\n",v);
    rbt_insert(T,v);
  }

  for(i = 0;i < 10;i++){
    v = rand() % MAX;
    printf("Busca %d\t",v);
    if(rbt_search(*T,v)){
      printf("Si se encuentra\n");
    }
    else {
      printf("No se encuentra\n");
      printf("Su predecesor es: %d \t y su antecesor es %d\n",
	     rbt_predecessor(*T,v),rbt_successor(*T,v));
    }
  }

  return 0;
}

void _rbt_insert_fixup(node *z){
  node *y;
  while (z->p != NULL && z->p->color == RED){
    if(z->p == z->p->p->left){
      y = z->p->p->right;
      if (y != NULL && y->color == RED){
	printf("Caso 1\n");
	z->p->color = BLACK;
	y->color = BLACK;
	z->p->p->color = RED;
	z = z->p->p;
      }
      else {
	if (z == z->p->right) {
	  printf("Caso 2\n");
	  z = z->p;
	  _rbt_left_rotate(&z);
	}
	printf("Caso 3\n");
	z->p->color = BLACK;
	z->p->p->color = RED;
	_rbt_right_rotate(&(z->p->p));
      }
    }
    else {
      y = z->p->p->left;
      if(y != NULL && y->color == RED){
	printf("Caso 1'\n");
	z->p->color = BLACK;
	y->color = BLACK;
	z->p->p->color = RED;
	z = z->p->p;
      }
      else {
	if (z == z->p->left) {
	  printf("Caso 2'\n");
	  z = z->p;
	  _rbt_right_rotate(&z);
	}
	printf("Caso 3'\n");
	z->p->color = BLACK;
	z->p->p->color = RED;
	_rbt_left_rotate(&(z->p->p));
      }
    }
  }
  if(z != NULL && z->p == NULL){
    z->color = BLACK;
  }
}

void _rbt_left_rotate(node **T){
  node *x,*y;
  x = *T;
  y = x->right;
  x->right = y->left;
  if (y->left != NULL)
    y->left->p = x;
  y->p = x->p;
  if (x->p == NULL)
    *T = y;
  else if (x == x->p->left)
    x->p->left = y;
  else x->p->right = y;
  y->left = x;
  x->p = y;
}

void _rbt_right_rotate(node **T){
  node *x,*y;
  x = *T;
  y = x->left;
  x->left = y->right;
  if (y->right != NULL)
    y->right->p = x;
  y->p = x->p;
  if (x->p == NULL)
    *T = y;
  else if (x == x->p->left)
    x->p->left = y;
  else x->p->right = y;
  y->right = x;
  x->p = y;
}

void _rbt_transplant(node **T,node *v){
  node *u = *T;
  if (u->p == NULL)
    *T = v;
  else if (u == u->p->left)
    u->p->left = v;
  else u->p->right = v;
  v->p = u->p;
}
