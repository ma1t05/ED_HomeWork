
/*
 * Author: Luis Maltos
 * Clase: Estructura de Datos en C++
 * Implementacion de arboles B
 * Se implementa:
 *  search, create, insert
 */

#include<stdlib.h>
#include<stdio.h>


#define TRUE 1
#define FALSE 0
#define B_TREE_T 3
#define DEBUG 0

struct B_node{
  int n;
  int *key;
  int leaf;
  struct B_node **c;
};

struct B_tree{
  int t;
  struct B_node *root;
};

typedef struct B_node B_node;
typedef struct B_tree B_tree;

void free_B_node(B_node *x){
  int i;
  if (!x->leaf)
    for(i = 0;i < x->n;i++) free_B_node(x->c[i]);
  free(x->key);
  free(x->c);
}

void free_B_tree(B_tree *T){
  free_B_node(T->root);
  free(T);
}

int b_tree_search(B_node *x,int k){
  int i = 0;
  while(i <= x->n && k > x->key[i]) i++;
  if (i <= x->n && k == x->key[i])
    return TRUE;
  if (x->leaf)
    return FALSE;
  return b_tree_search(x->c[i],k);
}

B_node* B_node_create(int t){
  int i;
  B_node *x;
  x = (B_node*)malloc(sizeof(B_node));
  x->n = 0;
  x->leaf = TRUE;
  x->key = (int*)malloc(sizeof(int)*(2*t - 1));
  x->c = (B_node**)malloc(sizeof(B_node*)*(2*t));
  for(i = 0;i < 2*t;i++) x->c[i] = NULL;
  return x;
}

B_tree* B_tree_create(int t){
  B_tree *T;
  B_node *x;
  x = B_node_create(t);
  T = (B_tree*)malloc(sizeof(B_tree));
  T->t = t;
  T->root = x;
  return T;
}

int B_tree_predecessor(B_node *x,int key){
  int i = x->n;
  int predecessor;
  while (i > 0 && key < x->key[i-1]) i--;
  if (x->leaf)
    return (i > 0 ? x->key[i-1] : key);
  predecessor = B_tree_predecessor(x->c[i],key);
  if (i > 0 && predecessor == key)
    predecessor = x->key[i-1];
  return predecessor;
}

int B_tree_successor(B_node *x,int key){
  int i = x->n;
  int successor;
  while (i > 0 && key < x->key[i-1]) i--;
  if (x->leaf)
    return (i < x->n ? x->key[i] : key);
  successor = B_tree_successor(x->c[i],key);
  if (i < x->n && successor == key)
    successor = x->key[i];
  return successor;
}

void B_tree_split_child(B_node *x,int i){
  int j;
  B_node *y,*z;
  printf("Comienza split child en posicion %d\n",i);
  z = B_node_create(B_TREE_T);
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
  for(j = x->n;j > i+1;j--)
    x->c[j+1] = x->c[j];
  x->c[i+1] = z;
  for(j = x->n;j > i;j--)
    x->key[j] = x->key[j-1];
  x->key[i] = y->key[B_TREE_T - 1];
  x->n++;
}

void B_tree_insert_nonfull(B_node *x,int key){
  int i = x->n;
#ifdef DEBUG
  printf("Entra a insert nonfull\n");
#endif
  if (x->leaf) {
    printf("Es hoja en nodo de tamaño %d\n",x->n);
    while (i > 0 && key < x->key[i-1])
      x->key[i] = x->key[--i]; // Precaucion!, validar orden de aplicacion
    printf("Se detiene en poscion %d\n",i);
    x->key[i] = key;
    x->n++;
    printf("Agrego clave %d en poscion %d\n",key,i);
  }
  else {
    while (i > 0 && key < x->key[i-1])
      i--;
    if (x->c[i]->n == 2 * B_TREE_T - 1) {
      B_tree_split_child(x,i);
      if (key > x->key[i]) i++;
    }
    B_tree_insert_nonfull(x->c[i],key);
  }
}

void B_tree_insert(B_tree *T,int key){
  B_node *r,*s;
  r = T->root;
#ifdef DEBUG
  printf("Agregando clave: %d\n",key);
  printf("Pasa por nodo con %d claves\n",r->n);
#endif
  if (r->n == 2 * T->t - 1) {
    printf("Aumenta un nivel el arbol\n");
    s = B_node_create(T->t);
    T->root = s;
    s->leaf = FALSE;
    s->n = 0;
    s->c[0] = r;
    B_tree_split_child(s,0);
    r = s;
  }
  B_tree_insert_nonfull(r,key);
#ifdef DEBUG
  printf("Clave: %d\tagregada\n",key);
#endif
}

void _B_tree_delete(B_node *x,int key){
  int i = x->n - 1,j;
  int k;
  B_node *y,*z;
  while (i > 0 && key < x->key[i])
    i--;
  if (key == x->key[i]){
    if (x->leaf){ // Case 1
      for(;i < x->n;i++)
	x->key[i-1] = x->key[i];
    }
    else { // Case 2
      y = x->c[i];
      z = x->c[i+1];
      if (y->n >= B_TREE_T) { //2a
	k = B_tree_predecessor(y,key);
	x->key[i] = k;
	_B_tree_delete(y,k);
      }
      else if (z->n >= B_TREE_T) { // 2b
	k = B_tree_successor(z,key);
	x->key[i] = k;
	_B_tree_delete(z,k);
      }
      else { // 2c
	i++;
	x->n--;
	for(;i < x->n;i++){
	  x->key[i-1] = x->key[i];
	  x->c[i] = x->c[i+1];
	}
	y->key[B_TREE_T] = key;
	for(i = 1;i < z->n;i++){
	  y->key[B_TREE_T + i] = z->key[i];
	  y->c[B_TREE_T + i] = z->c[i];
	  free(z->key);
	  free(z->c);
	}
	_B_tree_delete(y,key);
      }
    }
  }
  else { // Case 3
    y = (key < x->key[i] ? x->c[i] : x->c[++i]);
    if (y->n < B_TREE_T) {
      if (i < x->n && x->c[i+1]->n >= B_TREE_T) {
	z = x->c[i+1]; // Hermano con mas de t claves
	// Baja clave de x a y
	y->n++;
	y->key[B_TREE_T - 1] = x->key[i];
	y->c[B_TREE_T] = z->c[0];
	// Sube clave de hermano a x
	x->key[i] = z->key[0];
	// Suprimir clave 0 de hermano
	z->c[0] = z->c[1];
	for(j = 1;j < z->n;j++){
	  z->key[j-1] = z->key[j];
	  z->c[j] = z->c[j-1];
	}
	z->n--;
      }
      else if (i > 0 && x->c[i-1]->n >= B_TREE_T) {
	z = x->c[i-1]; // Hermano con mas de t claves
	// Baja clave de x a y
	y->n++;
	y->c[B_TREE_T] = y->c[B_TREE_T - 1];
	for(j = y->n - 1;j > 0;j--){
	  y->key[j] = y->key[j-1];
	  y->c[j] = y->c[j-1];
	}
	y->key[0] = x->key[i];
	y->c[0] = z->c[z->n];
	// Sube clave de hermano a x
	x->key[i] = z->key[z->n-1];
	// Suprime ultima clave a hermano
	z->key[z->n-1] = 0; // No es necesario borrar valor
	z->c[z->n] = NULL; // No es necesario apuntar nuevamente a null
	z->n--;
      }
      else {
	if (i < x->n)
	  z = x->c[i+1];
	else {
	  z = y;
	  y = x->c[i-1];
	}
	// Combinar z en y
	y->key[B_TREE_T - 1] = x->key[i];
	y->c[B_TREE_T] = z->c[0];
	for(j = 0;j < z->n;i++){
	  y->key[B_TREE_T + j] = z->key[j];
	  y->c[B_TREE_T + j + 1] = z->c[j+1];
	}
	free(z->key);
	free(z->c);
	free(z);
      }
    }
    _B_tree_delete(y,key);
  }
  
}

void B_tree_delete(B_tree *T,int key){
  int i;
  B_node *r,*s;
  r = T->root;
  if (r->leaf) {
    _B_tree_delete(r,key);
  }
  else {
    i = r->n;
    while (i > 0 && key < r->key[i-1])
      i--;
    if(key == r->key[i-1]){
      
    }
    else {
      if(r->c[i]->n >= T->t)
	_B_tree_delete(r->c[i],key);
      else {
      
      }
    }
  }
}

#define BLOCK_SIZE 5.0
#define HEIGHT 10.0
#define DELTA 3.0
#define EPSILON 0.25

float plot(B_node *x,FILE *nodes,FILE *edges,float xo,float y){
  int i;
  float xf,*z;
  if (x->leaf) {
    xo+=DELTA;
    for(i = 0; i < x->n; i++) {
      fprintf(nodes,"%f %f %d\n",xo,y,x->key[i]);
      xo+= BLOCK_SIZE;
    }
    xo-=BLOCK_SIZE;
    xo+=DELTA;
  }
  else {
    z = (float*)malloc(sizeof(float)*(x->n+2));
    xf = xo;
    for(i = 0;i <= x->n;i++){
      z[i] = xf;
      xf = plot(x->c[i],nodes,edges,xf,y - HEIGHT);
    }
    z[i] = xf;
    xo = (xo + xf) / 2;
    xo-= BLOCK_SIZE * x->n / 2;
    for(i = 0;i < x->n;i++){
      fprintf(nodes,"%f %f %d\n",xo,y,x->key[i]);
      xo+= BLOCK_SIZE;
    }
    xo-= BLOCK_SIZE * (x->n + 0.5);
    for(i = 0;i <= x->n;i++){
      fprintf(edges,"%f %f %f %f\n",xo,y,(z[i]+z[i+1])/2-xo,-HEIGHT+EPSILON);
      xo+= BLOCK_SIZE;
    }
    free(z);
    xo = xf;
  }
  return xo;
}

void gnuplot(B_tree *T){
  FILE *nodes,*edges;
  FILE *gnuPipe = popen("gnuplot","w");
  nodes = fopen("B_nodes.dat","w");
  edges = fopen("B_edges.dat","w");
  plot(T->root,nodes,edges,0.0,0.0);
  fclose(nodes);
  fclose(edges);

  fprintf(gnuPipe,"set term svg\n");
  fprintf(gnuPipe,"set output 'B_tree_%d.svg'\n",time(NULL));
  fprintf(gnuPipe,"unset key\n");
  fprintf(gnuPipe,"unset border\n");
  fprintf(gnuPipe,"unset yzeroaxis\n");
  fprintf(gnuPipe,"unset xtics\n");
  fprintf(gnuPipe,"unset ytics\n");
  fprintf(gnuPipe,"unset ztics\n");
  fprintf(gnuPipe,"plot ");
  fprintf(gnuPipe,"'B_nodes.dat' using 1:2:3 with labels");
  fprintf(gnuPipe,", 'B_edges.dat' using 1:2:3:4 with vectors filled head linecolor rgb \"dark-blue\"");
  fprintf(gnuPipe,"\n");
  pclose(gnuPipe);
}

int main(int argc,char *argv[]){
  int i,M;
  B_tree *T = B_tree_create(3);
  M = atoi(argv[1]);
  for(i=1;i<M;i++)
    B_tree_insert(T,i);
  gnuplot(T);
  free_B_tree(T);
  return 0;
}
