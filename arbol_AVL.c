
#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#define TRUE 1
#define FALSE 0
#define MAX 5000
#define NUM_REP 100

#define HEIGHT 10

struct AVL_tree{
  int altura;
  int profundidad;
  int claves;
  int valor;
  struct AVL_tree *izquierda;
  struct AVL_tree *derecha;
  struct AVL_tree *padre;
};

typedef struct AVL_tree avl;
/*
  Grafica para comparar una secuencia de operaciones 
  con balanceo y sin balanceo
  Contar cada llamada recursiva
*/
int busca_valor(avl**,int);
int agrega_valor(avl**,int);
int elimina_valor(avl**,int);
void avl_free(avl**);
int es_hoja(avl*);
void cheka_balance(avl**);

// Imprime arbol
void avl_printf(avl**);
void preorden(avl**);
void inorder(avl**);
void postorden(avl**);
void gnuplot(avl**);
void avl_plot(avl**,FILE*,FILE*,int,int);

avl* avl_crea_hoja(avl*);
void actualiza_altura(avl*);
int actualiza_profundidad(avl*);
void actualiza_claves(avl*);
void test_avl();

int main(int argc, char **argv){
  avl **arbol,**_arbol;
  int k,i,valor;
  int rep;
  long int pasos,total_bal,total_unbal;
  FILE *nodes = NULL,*edges = NULL;
  srand(time(NULL));

  arbol = (avl**)malloc(sizeof(avl*));
  _arbol = (avl**)malloc(sizeof(avl*));


  for(k = 100;k<MAX;k+=100){
    total_bal = 0;
    total_unbal = 0;
    rep = NUM_REP;
    while(rep-- > 0){
      *arbol = NULL;
      *_arbol = NULL;

      // Agrega Valores
      for(i = 0;i < k; i++){
	valor = rand()%MAX;
	agrega_valor(arbol,valor);
	_agrega_valor(_arbol,valor);
      }
  
      // Busca valores
      for(i = 0;i < MAX;i++){
	valor = i;
	pasos = busca_valor(arbol,valor);
	total_bal += (pasos > 0 ? pasos : -pasos);
	pasos = busca_valor(_arbol,valor);
	total_unbal += (pasos > 0 ? pasos : -pasos);
      }
    
      avl_free(arbol);
      avl_free(_arbol);
    }
    printf("%d %f %f\n",k,(float)total_bal/(float)NUM_REP,(float)total_unbal/(float)NUM_REP);
  }
  free(arbol);
  free(_arbol);

}

void test_avl(){
  avl **arbol,**_arbol;
  int k,i,valor;
  int pasos,total_bal,total_unbal;
  FILE *nodes = NULL,*edges = NULL;
  srand(time(NULL));

  arbol = (avl**)malloc(sizeof(avl*));
  *arbol = NULL;
  
  // Agrega Valores
  for(i = 0;i < k; i++){
    valor = rand()%MAX;
    agrega_valor(arbol,valor);
    _agrega_valor(_arbol,valor);
  }
  gnuplot(arbol);
  avl_printf(arbol);
  
  // Busca valores
  total_bal = 0;
  total_unbal = 0;
  for(i = 0;i < MAX;i++){
    valor = i;
    pasos = busca_valor(arbol,valor);
    total_bal += (pasos > 0 ? pasos : -pasos);
    pasos = busca_valor(_arbol,valor);
    total_unbal += (pasos > 0 ? pasos : -pasos);
  }
  printf("%d\t%d\n",total_bal,total_unbal);
  
  // Elimina valores
  printf("Elimina valores:\n");
  for(i = 0;i < 5;i++){
    valor = rand()%MAX;
    printf("elimina: %d",valor);
    elimina_valor(arbol,valor); 
  }
  printf("\n");
  avl_printf(arbol);
  preorden(arbol);
  inorder(arbol);
  postorden(arbol);
  
  for(i = 0;i < 10;i++){
    valor = rand()%MAX;
    printf("Busca el %d\n",valor);
    if(busca_valor(arbol,valor))
      printf("Si esta\n");
    else
      printf("No esta\n");
  }
  avl_free(arbol);
  free(arbol);
}

int busca_valor(avl **avl_tree,int valor){
  avl *tree;
  int encontro;
  if(avl_tree != NULL && *avl_tree != NULL){
    tree = *avl_tree;
    if(!es_hoja(tree)){
      //printf("[%d,%d]->",tree->valor,tree->profundidad);
      encontro = (valor < tree->valor ?
		  busca_valor(&(tree->izquierda),valor) :
		  busca_valor(&(tree->derecha),valor));
      if(encontro > 0){
	encontro++;
      }
      else {
	encontro--;
      }
      return encontro;
    }
    if(tree->valor == valor)
      return 1;
  }
  return -1;
}

int agrega_valor(avl **avl_tree,int valor){
  avl *tree;
  avl *hijo_izq,*hijo_der,*new_tree;
  int agrego;

  if(avl_tree == NULL) return FALSE;

  if(*avl_tree == NULL){
    *avl_tree = avl_crea_hoja(NULL);
    (*avl_tree)->valor = valor;
    return TRUE;
  }

  tree = *avl_tree;
  if(!es_hoja(tree)){
    if(valor < tree->valor)
      agrego = agrega_valor(&(tree->izquierda),valor);
    else
      agrego = agrega_valor(&(tree->derecha),valor);
  }
  else if (valor != tree->valor){
    tree->izquierda = avl_crea_hoja(tree);
    tree->derecha = avl_crea_hoja(tree);

    if(valor < tree->valor){
      tree->izquierda->valor = valor;
      tree->derecha->valor = tree->valor;
    }
    else {
      tree->izquierda->valor = tree->valor;
      tree->derecha->valor = valor;
      tree->valor = valor;
    }
    tree->altura++;
    return TRUE;
  }
  else agrego = FALSE;

  if(agrego == TRUE){
    cheka_balance(&tree);
    actualiza_altura(tree);
    *avl_tree = tree;
  }

  return agrego;
}

int _agrega_valor(avl **avl_tree,int valor){
  avl *tree;
  avl *hijo_izq,*hijo_der,*new_tree;
  int agrego;

  if(avl_tree == NULL) return FALSE;

  if(*avl_tree == NULL){
    *avl_tree = avl_crea_hoja(NULL);
    (*avl_tree)->valor = valor;
    return TRUE;
  }

  tree = *avl_tree;
  if(!es_hoja(tree)){
    if(valor < tree->valor)
      agrego = agrega_valor(&(tree->izquierda),valor);
    else
      agrego = agrega_valor(&(tree->derecha),valor);
  }
  else if (valor != tree->valor){
    tree->izquierda = avl_crea_hoja(tree);
    tree->derecha = avl_crea_hoja(tree);

    if(valor < tree->valor){
      tree->izquierda->valor = valor;
      tree->derecha->valor = tree->valor;
    }
    else {
      tree->izquierda->valor = tree->valor;
      tree->derecha->valor = valor;
      tree->valor = valor;
    }
    tree->altura++;
    return TRUE;
  }
  else agrego = FALSE;

  return agrego;
}

int elimina_valor(avl **avl_tree,int valor){
  // Pendiente Actualizar altura
  avl *tree,*padre,*hermano,*abuelo;
  int elimino;
  if(avl_tree == NULL || *avl_tree == NULL) return TRUE;
  tree = *avl_tree;
  if(!es_hoja(tree)){
    padre = tree;
    tree = (valor < tree->valor ? tree->izquierda : tree->derecha);
    if(es_hoja(tree)) {
      if(tree->valor == valor){
	free(tree);
	hermano = (valor < padre->valor ? padre->derecha : padre->izquierda);
	abuelo = padre->padre;
	free(padre);
	hermano->padre = abuelo;
	if(abuelo != NULL){
	  if(valor < abuelo->valor)
	    abuelo->izquierda = hermano;
	  else
	    abuelo->derecha = hermano;
	  actualiza_altura(abuelo);
	}
	else {
	  *avl_tree = hermano;
	}
	return TRUE;
      }
      return FALSE;
    }
    else {
      elimino = elimina_valor(&tree,valor);
      tree = padre;
    }
  }
  else {
    if(valor == tree->valor){
      padre = tree->padre;
      free(tree);
      if(padre != NULL){
	hermano = (valor < padre->valor ? padre->derecha : padre->izquierda);
	abuelo = padre->padre;
	free(padre);
	hermano->padre = abuelo;
	if(abuelo != NULL){
	  if(valor < abuelo->valor)
	    abuelo->izquierda = hermano;
	  else
	    abuelo->derecha = hermano;
	  actualiza_altura(abuelo);
	}
	else {
	  *avl_tree = hermano;
	}
      }
      return TRUE;
    }
    return FALSE;
  }

  if(elimino == TRUE){
    tree->claves--;
    if(!es_hoja(tree)){
      cheka_balance(&tree);
      actualiza_altura(tree);
      *avl_tree = tree;
    }    
  }

  return elimino;
}

/*
Realizar experimento que compare, listas enlazadas vs arboles
*/

void avl_free(avl **avl_tree){
  avl *tree;
  if(avl_tree == NULL) return;
  tree = *avl_tree;
  if(!es_hoja(tree)){
    avl_free(&(tree->izquierda));
    avl_free(&(tree->derecha));
  }
  //printf("free %d\n",tree->valor);
  free(tree);
}

int es_hoja(avl *tree){
  return (tree != NULL && tree->izquierda == NULL ? TRUE : FALSE);
}

/*
  A(u) >= A(v) + 2:
     A(A) >= A(B) => rotación simple a la derecha
     A(A) < A(w) => rotación doble izquierda-derecha
  A(u) <= A(v) - 2:
     A(A) >= A(B) => rotación simple a la izquierda
     A(B) < A(w) => rotación doble derecha-izquierda
*/
void cheka_balance(avl **avl_tree){
  avl *t,*u,*v,*A,*B,*padre;
  avl *w,*A1,*A2,*B1,*B2;
  if(avl_tree == NULL) return;
  t = *avl_tree;
  if (t == NULL) return;
  if(es_hoja(t)) return;

  u = t->izquierda;
  v = t->derecha;
  if(u->altura >= v->altura + 2){
    A = u->izquierda;
    B = u->derecha;
    padre = t->padre;
    if(A->altura >= B->altura){
      //      printf("rotación simple a la derecha\n");
      t->izquierda = B;
      B->padre = t;
      u->derecha = t;
      t->padre = u;
      u->padre = padre;
      *avl_tree = u;

      // Actualiza atributos
      actualiza_altura(t);
      actualiza_altura(u);
      actualiza_profundidad(u);
      actualiza_claves((padre != NULL ? padre : u));

    }
    else {
      //      printf("rotación doble izquierda-derecha\n");
      w = B;
      B1 = w->izquierda;
      B2 = w->derecha;
      u->derecha = B1;
      B1->padre = u;
      w->izquierda = u;
      u->padre = w;
      t->izquierda = B2;
      B2->padre = t;
      w->derecha = t;
      t->padre = w;
      w->padre = padre;
      *avl_tree = w;
	
      // Actualiza atributos
      actualiza_altura(u);
      actualiza_altura(t);
      actualiza_altura(w);
      actualiza_profundidad(w);
      actualiza_claves((padre != NULL ? padre : w));

    }
  }
  else if (u->altura <= v->altura - 2){
    A = v->izquierda;
    B = v->derecha;
    padre = t->padre;
    if(A->altura <= B->altura){
      //      printf("rotación simple a la izquierda\n");
      t->derecha = A;
      A->padre = t;
      v->izquierda = t;
      v->padre = padre;
      t->padre = v;
      *avl_tree = v;

      // Actualiza atributos
      actualiza_altura(t);
      actualiza_altura(v);
      actualiza_profundidad(v);
      actualiza_claves((padre != NULL ? padre : v));

    }
    else {
      //      printf("rotación doble derecha-izquierda\n");
      w = A;
      A1 = w->izquierda;
      A2 = w->derecha;
      t->derecha = A1;
      A1->padre = t;
      w->izquierda = t;
      t->padre = w;
      v->izquierda = A2;
      A2->padre = v;
      w->derecha = v;
      v->padre = w;
      w->padre = padre;
      *avl_tree = w;

      // Actualiza atributos
      actualiza_altura(t);
      actualiza_altura(v);
      actualiza_altura(w);
      actualiza_profundidad(w);
      actualiza_claves((padre != NULL ? padre : w));

    }
  }
}
/**/

void avl_printf(avl **avl_tree){
  avl *tree;
  int i;
  if(avl_tree == NULL) return;
  tree = *avl_tree;
  
  if(!es_hoja(tree)){
    printf("%d,%d:%d\t",tree->valor,tree->profundidad,tree->altura);
    avl_printf(&(tree->derecha));
    for(i = 0;i <= tree->profundidad;i++) 
      printf("|\t");
    avl_printf(&(tree->izquierda));
  }
  else {
    printf("(%d,%d:%d)\n",tree->valor,tree->profundidad,tree->altura);
  }
}

void preorden(avl **avl_tree){
  if(avl_tree == NULL || *avl_tree == NULL) return;
  printf("%d|",(*avl_tree)->valor);
  if(!es_hoja(*avl_tree)){
    preorden(&((*avl_tree)->izquierda));
    preorden(&((*avl_tree)->derecha));
  }
  if((*avl_tree)->padre == NULL)
    printf("\n");
}

void inorder(avl **avl_tree){
  if(avl_tree == NULL || *avl_tree == NULL) return;
  if(!es_hoja(*avl_tree))
    inorder(&((*avl_tree)->izquierda));
  printf("%d|",(*avl_tree)->valor);
  if(!es_hoja(*avl_tree)){
    inorder(&((*avl_tree)->derecha));
  }
  if((*avl_tree)->padre == NULL)
    printf("\n");
}

void postorden(avl **avl_tree){
  if(avl_tree == NULL || *avl_tree == NULL) return;
  if(!es_hoja(*avl_tree)){
    postorden(&((*avl_tree)->izquierda));
    postorden(&((*avl_tree)->derecha));
  }
  printf("%d|",(*avl_tree)->valor);
  if((*avl_tree)->padre == NULL)
    printf("\n");
}

void actualiza_altura(avl *t){
  t->altura = (t->izquierda->altura > t->derecha->altura ?
	       t->izquierda->altura : t->derecha->altura) + 1;
}

void actualiza_claves(avl *t){
  if(!es_hoja(t)){
    actualiza_claves(t->izquierda);
    actualiza_claves(t->derecha);
    t->claves = t->izquierda->claves + t->derecha->claves;
  }
  else {
    t->claves = 1;
  }
}

int actualiza_profundidad(avl *t){
  t->profundidad = (t->padre == NULL ? 0 : t->padre->profundidad +1);
  if(!es_hoja(t)){
    actualiza_profundidad(t->izquierda);
    actualiza_profundidad(t->derecha);
  }
}

avl* avl_crea_hoja(avl *padre){
  avl *hoja;
  hoja = (avl*)malloc(sizeof(avl));
  hoja->altura = 1;
  hoja->profundidad = (padre == NULL ? 0 : padre->profundidad + 1);
  hoja->claves = 1;
  hoja->izquierda = NULL;
  hoja->derecha = NULL;
  hoja->padre = padre;
  return hoja;
}

void avl_plot(avl **tree,FILE *nodes,FILE *edges,int min,int max){
  avl *t;
  int x,y;
  if(tree == NULL) return;
  t = *tree;
  if (t == NULL) return;
  x = (es_hoja(t) ?
       (min + max) / 2 :
       min + (max - min)*(t->izquierda->claves)/(t->claves));
  y = -HEIGHT * t->profundidad;
  fprintf(nodes,"%d %d\n",x,y);
  if(!es_hoja(t)){
    avl_plot(&(t->izquierda),nodes,edges,min,x);
    avl_plot(&(t->derecha),nodes,edges,x,max);
  }
  if(t->padre != NULL){
    fprintf(edges,"%d %d ",x,y);
    if(t == t->padre->izquierda){
      fprintf(edges,"%d %d\n",max-x,HEIGHT);
    }
    else {
      fprintf(edges,"%d %d\n",min-x,HEIGHT);
    }
  }
}

// Plot AVL Tree
void gnuplot(avl** t){
  FILE *nodes,*edges;
  FILE *gnuPipe = popen("gnuplot","w");
  nodes = fopen("nodos.dat","w");
  edges = fopen("aristas.dat","w");
  actualiza_claves(*t);
  avl_plot(t,nodes,edges,-400,400);
  fclose(nodes);
  fclose(edges);
  
  fprintf(gnuPipe,"set term pngcairo\n");
  fprintf(gnuPipe,"set output 'AVL_tree_%d.png'\n",time(NULL));
  fprintf(gnuPipe,"unset key\n");
  fprintf(gnuPipe,"unset border\n");
  fprintf(gnuPipe,"unset yzeroaxis\n");
  fprintf(gnuPipe,"unset xtics\n");
  fprintf(gnuPipe,"unset ytics\n");
  fprintf(gnuPipe,"unset ztics\n");
  fprintf(gnuPipe,"set style fill transparent solid 1.0\n");
  fprintf(gnuPipe,"plot 'aristas.dat' using 1:2:3:4 with vectors filled heads linecolor rgb \"dark-blue\", 'nodos.dat' using 1:2:(7) with circles lc rgb \"blue\"\n");
 
}
