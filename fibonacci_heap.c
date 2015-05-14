/*
 * Autor: Luis Maltos
 * Asignatura: Estructura de Datos en C++
 * Tarea: Fobinacci Heaps
 * Se implementa:
 *   make-heap, insert, extract-min, union,
 *   decrease-key, delete
 */

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <math.h>

#define TRUE 1
#define FALSE 0
#define MINUS_INF INT_MIN

struct fibonacci_node {
  struct fibonacci_node *p,*child;
  struct fibonacci_node *left,*right;
  int degree;
  int mark;
  int key;
};

struct fibonacci_heap {
  struct fibonacci_node *min;
  int n;
};

typedef struct fibonacci_node fib_node;
typedef struct fibonacci_heap fib_heap;

fib_heap* make_fib_heap ();
fib_node* make_fib_node(int);
void fib_heap_insert(fib_heap*,int);
fib_heap* fib_heap_union(fib_heap*,fib_heap*);
fib_node* fib_heap_extract_min(fib_heap*);
void consolidate (fib_heap*);
void fib_heap_link (fib_heap*,fib_node*,fib_node*);
void fib_heap_decrease_key (fib_heap*,fib_node*,int);
void fib_cut (fib_heap*,fib_node*,fib_node*);
void fib_cascading_cut (fib_heap*,fib_node*);
void fib_heap_delete(fib_heap*,fib_node*);
void fib_heap_test ();

fib_heap* make_fib_heap(){
  fib_heap *H;
  H = (fib_heap*)malloc(sizeof(fib_heap));
  H->n = 0;
  H->min = NULL;
  return H;
}

fib_node* make_fib_node(int key){
  fib_node *x;
  x = (fib_node*)malloc(sizeof(fib_node));
  x->p = NULL;
  x->child = NULL;
  x->left = x;
  x->right = x;
  x->degree = 0;
  x->mark = FALSE;
  x->key = key;
  return x;
}

void fib_heap_insert(fib_heap *H,int key){
  fib_node *x = make_fib_node(key);
  if (H->min == NULL) {
    H->min = x;
  }
  else {
    // Inserta x en la lista de raices de H
    if (x->key < H->min->key)
      H->min = x;
    H->n++;
  }
}

fib_heap* fib_heap_union(fib_heap *H1,fib_heap *H2){
  fib_heap* H = make_fib_heap();
  H->min = H1->min;
  H->min->left->right = H2->min->right;
  H2->min->right->left = H->min->left;
  if (H1->min == NULL || (H2->min != NULL && H2->min->key < H1->min->key))
    H->min = H2->min;
  H->n = H1->n + H2->n;
  free(H1);
  free(H2);
  return H;
}

fib_node* fib_heap_extract_min(fib_heap *H){
  int i;
  fib_node *x,*z = H->min;
  if (z != NULL) {
    if (z->degree > 0) {
      x = z->child;
      for (i = 0;i < z->degree;i++) {
	x->p = NULL;
	x = x->right;
      }
      z->left->right = x;
      x->left->right = z->right;
      z->right->left = x->left;
      x->left = z->left;
      z->child = NULL;
    }
    if (z == z->right)
      H->min = NULL;
    else {
      H->min = z->right;
      consolidate(H);
    }      
    H->n--;
  }
  return z;
}

void consolidate (fib_heap *H){
  fib_node **A,*w,*x,*y;
  int i,D,d;
  // Pendiente: calcular D
  D = log(H->n);
  A = (fib_node**)malloc(sizeof(fib_node*)*D);
  for (i = 0;i < D;i++) A[i] = NULL;
  w = H->min;
  for (i = 0;i < D;i++) {
    x = w;
    w = w->right;
    d = x->degree;
    while (A[d] != NULL) {
      y = A[d];
      if (x->key > y->key) {
	fib_heap_link(H,x,y);
	x = y;
      }
      else {
	fib_heap_link(H,y,x);
      }
      A[d] = NULL;
      d++;
    }
    A[d] = x;
  }
  H->min = NULL;
  for (i = 0;i < D;i++) {
    if (A[i] != NULL) {
      if (H->min == NULL) {
	H->min = A[i];
      }
      else {
	// Inserta nodo A[i] a lista de raices de H
	A[i]->left = H->min->left;
	H->min->left->right = A[i];
	A[i]->right = H->min;
	H->min->left = A[i];
	if (A[i]->key < H->min->key) {
	  H->min = A[i];
	}
      }
    }
  }
  free(A);
}

void fib_heap_link(fib_heap *H,fib_node *y,fib_node *x){
  fib_node *aux;
  aux = y->left;
  aux->right = y->right;
  y->right->left = aux;
  y->mark = FALSE;
  y->p = x;
  if (x->child != NULL) {
    y->right = x->child;
    x->child->left->right = y;
    y->left = x->child->left;
    x->child->left = y;
  }
  else {
    x->child = y;
    y->left = y;
   y->right = y;
  }
  x->degree++;
}

void fib_heap_decrease_key (fib_heap *H,fib_node *x,int k) {
  fib_node *y;
  if (k > x->key) {
    printf("new key is greater than current key\n");
    return;
  }
  x->key = k;
  y = x->p;
  if (y != NULL && x->key < y->key) {
    fib_cut(H,x,y);
    fib_cascading_cut(H,y);
  }
  if (x->key < H->min->key)
    H->min = x;
}

void fib_cut (fib_heap *H,fib_node *x,fib_node *y) {
  /* remueve x de la lista de hijos de y */
  if (x == y->child) {
    y->child = (x->left == x ? NULL : x->left);
  }
  x->left->right = x->right;
  x->right->left = x->left;
  /* decrementa el grado de y */
  y->degree--;
  /* agrega x a la lista de raizes de H */
  H->min->left->right = x;
  x->left = H->min->left;
  x->right = H->min;
  H->min->left = x;
  x->p = NULL;
  x->mark = FALSE;
}

void fib_cascading_cut (fib_heap *H,fib_node *y) {
  fib_node *z = y->p;
  if (z != NULL) {
    if (y->mark == FALSE)
      y->mark = TRUE;
    else  {
      fib_cut(H,y,z);
      fib_cascading_cut(H,z);
    }
  }
}

void fib_heap_delete (fib_heap *H, fib_node *x) {
  fib_heap_decrease_key(H,x,MINUS_INF);
  fib_heap_extract_min(H);
}

void fib_heap_test () {
  
}
