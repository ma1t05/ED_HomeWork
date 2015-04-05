
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
#include<time.h>

#define TRUE 1
#define FALSE 0
#define RED 'r'
#define BLACK 'b'
#define MAX 1000
#define HEIGHT 5

struct node{
  char color;
  int key;
  struct node *left,*right,*p;
};

struct red_black_tree{
  int elements;
  struct node *root;
  struct node *nil;
};

typedef struct node node;
typedef struct red_black_tree rbt;

void _rbt_insert_fixup(rbt*,node*);
void _rbt_delete_fixup(rbt*,node*);
void _rbt_left_rotate(rbt*,node*);
void _rbt_right_rotate(rbt*,node*);
void _rbt_transplant(rbt*,node*,node*);
void rbt_printf(rbt*,node*,int);
void rbt_plot(rbt*,node*,FILE*,FILE*,FILE*,int,int,int);
void rbt_gnuplot(rbt*);

node* rbt_node(rbt *T,node *p,int key){
  node *x;
  x = (node*)malloc(sizeof(node));
  x->color = RED;
  x->key = key;
  x->left = T->nil;
  x->right = T->nil;
  x->p = p;
  return x;
}

rbt* _rbt_(){
  rbt* T;
  T = (rbt*)malloc(sizeof(rbt));
  T->elements = 0;
  T->nil = (node*)malloc(sizeof(node));
  T->nil->color = BLACK;
  T->nil->left = NULL;
  T->nil->right = NULL;
  T->nil->p = NULL;
  T->root = T->nil;
  return T;
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

int rbt_search(rbt *T,int key){
  node *x;
  x = T->root;
  while (x != T->nil) {
    if (key < x->key)
      x = x->left;
    else if (key > x->key)
      x = x->right;
    else return TRUE;
  }
  return FALSE;
}

int rbt_predecessor(rbt *T,int key){
  node *x,*y;
  y = T->nil;
  x = T->root;
  while(x != T->nil) {
    if (key > x->key){
      y = x;
      x = x->right;
    }
    else x = x->left;
  }
  return y->key;
}

int rbt_successor(rbt *T,int key){
  node *x,*y;
  x = T->root;
  y = x;
  while(x != T->nil){
    if (key < x->key) {
      y = x;
      x = x->left;
    }
    else x = x->right;
  }
  return y->key;
}

int rbt_minimum(rbt *T){
  node *x;
  x = T->root;
  while (x != T->nil)
    x = x->left;
  return x->key;
}

int rbt_maximum(node *T){
  if(T->right != NULL)
    return rbt_maximum(T->right);
  return T->key;
}

int rbt_insert(rbt *T,int key){
  node *x,*y,*z;
  y = T->nil;
  x = T->root;
  while(x != T->nil){
    printf("%d->",x->key);
    y = x;
    if(key < x->key)
      x = x->left;
    else x = x->right;
  }
  printf("\n");
  z = rbt_node(T,y,key);
  if(y == T->nil){
    printf("Agrega raiz\n");
    T->root = z;
  }
  else if (key < y->key) {
    printf("Agrega a la izquierda\n");
    y->left = z;
  }
  else if (key > y->key) {
    printf("Agrega a la derecha\n");
    y->right = z;
  }
  else {
    free(z);
    return FALSE;
  }
  _rbt_insert_fixup(T,z);
  T->elements++;
  return TRUE;
}

int rbt_delete(rbt *T,int key){
  char color;
  node *x,*y,*z = _rbt_search(T->root,key);
  if (z == NULL) return FALSE;
  printf("Eliminando nodo %d\n",key);
  y = z;
  color = y->color;
  if (z->left == T->nil) {
    printf("Sin hijo izquierdo\n");
    x = z->right;
    _rbt_transplant(T,z,z->right);
  }
  else if (z->right == T->nil) {
    printf("Sin hijo derecho\n");
    x = z->left;
    _rbt_transplant(T,z,z->left);
  }
  else {
    printf("Busca reemplazo\n");
    y = z->right;
    while (y->left != T->nil) y = y->left;
    printf("El reemplazo es %d\n",y->key);
    color = y->color;
    x = y->right;
    if (y->p == z)
      x->p = y;
    else {
      _rbt_transplant(T,y,y->right);
      y->right = z->right;
      y->right->p = y;
    }
    _rbt_transplant(T,z,y);
    y->left = z->left;
    y->left->p = y;
    y->color = z->color;
  }
  free(z);
  T->elements--;
  if (color == BLACK) {
    _rbt_delete_fixup(T,x);
  }
  return TRUE;
}

int main(int argc,char *argv[]){
  int i,v;
  int N;
  rbt *T;
  srand(time(NULL));
  T = _rbt_();

  if(argc > 1){
    N = atoi(argv[1]);
  }
  else N = 50;

  for(i = 0;i < N;i++){
    v = rand() % MAX;
    printf("Inserta %d\n",v);
    rbt_insert(T,v);
  }
  //rbt_gnuplot(T);

  for(i = 0;i < N;i++){
    v = rand() % MAX;
    printf("Busca %d\t",v);
    if(rbt_search(T,v)){
      printf("Si se encuentra\n");
    }
    else {
      printf("No se encuentra\n");
      printf("Su predecesor es: %d \t y su sucecesor es %d\n",
	     rbt_predecessor(T,v),rbt_successor(T,v));
    }
  }

  i = 0;
  while (i < N) {
    v = rand() % MAX;
    if (rbt_search(T,v)){
      printf("Elimina %d\n",v);
      rbt_delete(T,v);
      i += 10;
    }
  }
  //rbt_gnuplot(T);
    
  return 0;
}

void _rbt_insert_fixup(rbt *T,node *z){
  node *y;
  printf("Comienza reparacion\n");
  while (z->p != NULL && z->p->color == RED){
    if(z->p == z->p->p->left){
      y = z->p->p->right;
      if (y->color == RED){
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
	  _rbt_left_rotate(T,z);
	}
	printf("Caso 3\n");
	z->p->color = BLACK;
	z->p->p->color = RED;
	_rbt_right_rotate(T,z->p->p);
      }
    }
    else {
      y = z->p->p->left;
      if(y->color == RED){
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
	  _rbt_right_rotate(T,z);
	}
	printf("Caso 3'\n");
	z->p->color = BLACK;
	z->p->p->color = RED;
	_rbt_left_rotate(T,z->p->p);
      }
    }
  }
  T->root->color = BLACK;
}

void _rbt_delete_fixup(rbt *T,node *x){
  node *w;
  printf("Comienza reparacion por eliminiacion\n");
  while (x->p != T->nil && x->color == BLACK){
    if (x == x->p->left) {
      w = x->p->right;
      if (w->color == RED) {
	printf("Caso 1\n");
	w->color = BLACK;
	x->p->color = RED;
	_rbt_left_rotate(T,x->p);
	w = x->p->right;
      }
      if (w->left->color == BLACK && w->right->color == BLACK) {
	printf("Caso 2\n");
	w->color = RED;
	x = x->p;
      }
      else {
	if (w->right->color == BLACK) {
	  printf("Caso 3\n");
	  w->left->color = BLACK;
	  w->color = RED;
	  _rbt_right_rotate(T,w);
	  w = x->p->right;
	}
	printf("Caso 4\n");
	w->color = x->p->color;
	x->p->color = BLACK;
	w->right->color = BLACK;
	printf("La raiz es: %d\n",T->root->key);
	_rbt_left_rotate(T,x->p);
	printf("Ahora la raiz es: %d\n",T->root->key);
	x = T->root;
      }
    }
    else {
      w = x->p->left;
      if (w != NULL && w->color == RED) {
	printf("Caso 1'\n");
	w->color = BLACK;
	x->p->color = RED;
	_rbt_right_rotate(T,x->p);
	w = x->p->left;
      }
      if (w->right->color == BLACK && w->left->color == BLACK) {
	printf("Caso 2'\n");
	w->color = RED;
	x = x->p;
      }
      else {
	if (w->left->color == BLACK) {
	  printf("Caso 3'\n");
	  w->right->color = BLACK;
	  w->color = RED;
	  _rbt_left_rotate(T,w);
	  w = x->p->left;
	}
	printf("Caso 4'\n");
	w->color = x->p->color;
	x->p->color = BLACK;
	w->left->color = BLACK;
	_rbt_right_rotate(T,x->p);
	x = T->root;
      }
    }
  }
  x->color = BLACK;
}

void _rbt_left_rotate(rbt *T,node *x){
  node *y;
  y = x->right;
  x->right = y->left;
  if (y->left != T->nil)
    y->left->p = x;
  y->p = x->p;
  if (x->p == T->nil)
    T->root = y;
  else if (x == x->p->left)
    x->p->left = y;
  else x->p->right = y;
  y->left = x;
  x->p = y;
}

void _rbt_right_rotate(rbt *T,node *x){
  node *y;
  y = x->left;
  x->left = y->right;
  if (y->right != T->nil)
    y->right->p = x;
  y->p = x->p;
  if (x->p == T->nil)
    T->root = y;
  else if (x == x->p->left)
    x->p->left = y;
  else x->p->right = y;
  y->right = x;
  x->p = y;
}

void _rbt_transplant(rbt *T,node *u,node *v){
  if (u->p == NULL)
    T->root = v;
  else if (u == u->p->left)
    u->p->left = v;
  else u->p->right = v;
  v->p = u->p;
}

void rbt_printf(rbt *T,node *n,int tabs){
  int i;
  if (n->color == BLACK) printf("*");
  printf("%d\t",n->key);
  if (n->right != T->nil)
    rbt_printf(T,n->right,tabs + 1);
  printf("\n");
  if (n->left != T->nil) {
    for(i = 0;i < tabs;i++) printf("\t");
    rbt_printf(T,n->left,tabs + 1);
  }
}

void rbt_gnuplot(rbt *T){
  FILE *black_nodes,*red_nodes,*edges;
  FILE *gnuPipe = popen("gnuplot","w");
  black_nodes = fopen("nodos_negros.dat","w");
  red_nodes = fopen("nodos_rojos.dat","w");
  edges = fopen("aristas.dat","w");

  rbt_plot(T,T->root,black_nodes,red_nodes,edges,-HEIGHT*T->elements,HEIGHT*T->elements,0);
  fclose(black_nodes);
  fclose(red_nodes);
  fclose(edges);
  
  fprintf(gnuPipe,"set term svg\n");
  fprintf(gnuPipe,"set output 'red_black_tree_%d.svg'\n",time(NULL));
  fprintf(gnuPipe,"unset key\n");
  fprintf(gnuPipe,"unset border\n");
  fprintf(gnuPipe,"unset yzeroaxis\n");
  fprintf(gnuPipe,"unset xtics\n");
  fprintf(gnuPipe,"unset ytics\n");
  fprintf(gnuPipe,"unset ztics\n");
  fprintf(gnuPipe,"set style fill transparent solid 1.0\n");
  fprintf(gnuPipe,"lab(String,Size) = sprintf(\"{/=%d %s}\", Scale(Size), String)\n");
  fprintf(gnuPipe,"plot ");
  fprintf(gnuPipe,"'aristas.dat' using 1:2:3:4 with vectors filled heads linecolor rgb \"dark-blue\"");
  fprintf(gnuPipe,", 'nodos_negros.dat' using 1:2:(50) with circles lc rgb \"black\"");
  fprintf(gnuPipe,", 'nodos_negros.dat' using 1:2:3 with labels textcolor rgb \"white\"");
 fprintf(gnuPipe,", 'nodos_rojos.dat' using 1:2:(50) with circles lc rgb \"red\"");
  fprintf(gnuPipe,", 'nodos_rojos.dat' using 1:2:3 with labels textcolor rgb \"black\"");
  fprintf(gnuPipe,"\n");
 

}

void rbt_plot(rbt *T,node *n,FILE *black_nodes,FILE *red_nodes,FILE *edges,int min,int max,int ybase){
  int x,y;
  int xmin,xmax,ymin;
  if(n == T->nil) return;
  x = (min + max) / 2;
  y = ybase - HEIGHT;
  if (n->color == BLACK)
    fprintf(black_nodes,"%d %d %d\n",x,y,n->key);
  else 
    fprintf(red_nodes,"%d %d %d\n",x,y,n->key);
  if (n->left != T->nil){
    fprintf(edges,"%d %d %d %d\n",x,y,(min + x)/2-x,-HEIGHT);
    rbt_plot(T,n->left,black_nodes,red_nodes,edges,min,x,ybase - HEIGHT);
  }
  if (n->right != T->nil){
    fprintf(edges,"%d %d %d %d\n",x,y,(max + x)/2-x,-HEIGHT);
    rbt_plot(T,n->right,black_nodes,red_nodes,edges,x,max,ybase - HEIGHT);
  }
}
