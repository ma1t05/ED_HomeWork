 
#include<stdlib.h>
#include<stdio.h>
#include<time.h>

#define MAX 1000
#define TRUE 1
#define FALSE 0

#define ENCONTRO 1
#define NO_ENCONTRO 2
#define NO_HAY 3
#define HEIGHT 10

struct nodoDeArbol{
  int valor;
  int Num_Hijos;
  int claves;
  struct nodoDeArbol* padre;
  struct nodoDeArbol** hijos;
};

typedef struct nodoDeArbol nodo;

int tree_insertar(nodo**,int);
int tree_buscar(nodo**,int);
void tree_eliminar(nodo**,int);

nodo* crea_nodo(nodo*,int);
void aumenta_espacio_hijos(nodo*);
void tree_free(nodo*);
void reduce_espacio_hijos(nodo*);

void test_tree(int);
void gnuplot(nodo**);
void tree_plot(nodo**,FILE*,FILE*,int,int,int);

int main(int argc, char** argv){
  if(argc > 1){
    test_tree(atoi(argv[1]));
  }
  else {
    test_tree(100);
  }
  return 0;
}

void test_tree(int n){
  int i,valor;
  nodo **arbol;
  
  arbol = (nodo**)malloc(sizeof(nodo*));
  *arbol = NULL;
  srand(time(NULL));
  for(i = 0;i < n;i++){
    valor = rand()%MAX;
    tree_insertar(arbol,valor);
  }
  for(i = 0;4*i < n;i++){
    valor = rand()%MAX;
    tree_eliminar(arbol,valor);
  }
  gnuplot(arbol);
  for(i = 1;i < n;i++){
    valor = rand()%MAX;
    printf("Busca el %d\t",valor);
    if(tree_buscar(arbol,i) == TRUE){
      printf("lo encontro\n");
    }
    else {
      printf("no lo encontro\n");
    }
  }

  tree_free(*arbol);
  free(arbol);
}

int tree_insertar(nodo **arbol,int valor){
  int i,flag;
  nodo *n,*root;
  nodo **hijos;
  if(*arbol == NULL){
    *arbol = crea_nodo(NULL,valor);
    return 1;
  }
  n = *arbol;
  if(valor > n->valor){
    // Actualiza raiz
    root = crea_nodo(NULL,valor);
    aumenta_espacio_hijos(root);
    (root->hijos)[0] = *arbol;
    *arbol = root;
    return 1;
  }
  else if (valor < n->valor){
    hijos = n->hijos;
    flag = NO_HAY;
    for(i = 0;i < n->Num_Hijos;i++){
      if(hijos[i] != NULL){
	if(hijos[i]->valor > valor){
	  tree_insertar(&hijos[i],valor);
	  flag = ENCONTRO;
	  break;
	}
	else if (hijos[i]->valor == valor){
	  return;
	}
      }
      else {
	flag = NO_ENCONTRO;
      }
    }
    switch (flag){
    case NO_HAY:
      aumenta_espacio_hijos(n);
    case NO_ENCONTRO:
      hijos = n->hijos;
      for(i = 0;i < n->Num_Hijos;i++){
	if(hijos[i] == NULL)
	  break;
      }
      hijos[i] = crea_nodo(n,valor);
      break;
    default:
      break;
    }
  }
  else {
    return -1;
  }
}

int tree_buscar(nodo **arbol,int valor){
  int i;
  nodo *n = *arbol;
  if(n == NULL) return FALSE;
  if(n->valor > valor){
    for(i = 0;i < n->Num_Hijos;i++){
      if((n->hijos)[i] != NULL){
	if((n->hijos)[i]->valor > valor){
	  return tree_buscar(&(n->hijos)[i],valor);
	}
	else if((n->hijos)[i]->valor == valor)
	  return TRUE;
      }      
    }
  }
  else if(n->valor < valor){
    return FALSE;
  }
  else return TRUE;
}

void tree_eliminar(nodo **arbol,int valor){
  int i;
  nodo **hijos,*hijo;
  nodo *n = *arbol;
  if(n == NULL || n->valor < valor) return;
  if(n->valor > valor){
    if(n->Num_Hijos > 0){
      hijos = n->hijos;
      for(i = 0;i < n->Num_Hijos;i++){
	if(hijos[i] != NULL && hijos[i]->valor >= valor){
	  break;
	}
      }
      if(i < n->Num_Hijos){
	if(hijos[i]->valor == valor){
	  tree_eliminar(&(hijos[i]),valor);
	  reduce_espacio_hijos(n);
	}
	else {
	  tree_eliminar(&(hijos[i]),valor);
	}
      }
      else return;
    }
    else return;
  }
  else {
    if(n->Num_Hijos > 0){
      hijos = n->hijos;
      for(i = n->Num_Hijos;i > 0;i--){
	if(hijos[i-1] != NULL)
	  break;
      }
      hijo = hijos[i-1];
      
      n->valor = hijo->valor;
      hijo->valor = valor;
      tree_eliminar(&(n->hijos)[i-1],valor);
      reduce_espacio_hijos(n);
    }
    else {
      tree_free(n);
      *arbol = NULL;
    }
  }
}

nodo* crea_nodo(nodo* padre,int valor){
  nodo* n;
  n = (nodo*)malloc(sizeof(nodo));
  n->valor = valor;
  n->Num_Hijos = 0;
  n->claves = 0;
  n->hijos = NULL;
  n->padre = padre;
  return n;
}

void aumenta_espacio_hijos(nodo *n){
  int i,k,Num_Hijos;
  nodo **hijos,*hijo;
  Num_Hijos = (n->Num_Hijos > 0 ? 2*n->Num_Hijos : 1);
  hijos = (nodo**)malloc(sizeof(nodo*)*Num_Hijos);
  k = 0;
  for(i = 0;i < n->Num_Hijos;i++){
    if((n->hijos)[i] != NULL)
      hijos[k++] = (n->hijos)[i];
  }
  for(;k < Num_Hijos;k++)
    hijos[k] = NULL;
  free(n->hijos);
  n->hijos = hijos;
  n->Num_Hijos = Num_Hijos;
}

void tree_free(nodo *n){
  int i;
  if(n != NULL){
    if(n->Num_Hijos > 0){
      for(i = 0;i < n->Num_Hijos;i++)
	tree_free((n->hijos)[i]);
      free(n->hijos);
    }
    printf("elimina nodo: %d\n",n->valor);
    free(n);
  }
}

void reduce_espacio_hijos(nodo *n){
  int i,k;
  nodo **hijos;
  if(n != NULL && n->Num_Hijos > 0){
    k = 0;
    printf("Reduce de %d a ",n->Num_Hijos);
    for(i = 0;i < n->Num_Hijos;i++)
      if(n->hijos[i] != NULL) k++;
    if(3*k < n->Num_Hijos){
      if(k > 0) {
	hijos = (nodo**)malloc(sizeof(nodo*)*n->Num_Hijos / 2);
	k = 0;
	for(i = 0;i < n->Num_Hijos;i++){
	  if(n->hijos[i] != NULL)
	    hijos[k++] = n->hijos[i];
	}
	free(n->hijos);
	n->hijos = hijos;
	n->Num_Hijos /= 2;
      }
      else {
	free(n->hijos);
	n->hijos = NULL;
	n->Num_Hijos = 0;
      }
    }
    printf("%d\n",n->Num_Hijos);
  }
}

void gnuplot(nodo** arbol){
  FILE *nodos,*aristas;
  FILE *gnuPipe = popen("gnuplot","w");
  nodos = fopen("nodos.dat","w");
  aristas = fopen("aristas.dat","w");
  tree_plot(arbol,nodos,aristas,-400,400,0);
  fclose(nodos);
  fclose(aristas);
  
  fprintf(gnuPipe,"set term pngcairo\n");
  fprintf(gnuPipe,"set output 'tree_%d.png'\n",time(NULL));
  fprintf(gnuPipe,"unset key\n");
  fprintf(gnuPipe,"unset border\n");
  fprintf(gnuPipe,"unset yzeroaxis\n");
  fprintf(gnuPipe,"unset xtics\n");
  fprintf(gnuPipe,"unset ytics\n");
  fprintf(gnuPipe,"unset ztics\n");
  fprintf(gnuPipe,"set style fill transparent solid 1.0\n");
  //  fprintf(gnuPipe,"etiqueta(String,Size) = sprintf(\"{/=%d %s}\", Scale(Size), String)\n");
  fprintf(gnuPipe,"plot 'aristas.dat' using 1:2:3:4 with vectors filled heads linecolor rgb \"dark-blue\"");
  fprintf(gnuPipe,", 'nodos.dat' using 1:2:(log(-$2)) with circles lc rgb \"blue\"");
  //  fprintf(gnuPipe,"', nodos.dat' using 1:2:(etiqueta(stringcolumn(3),$4)) with labels point offset character 0,character 1 textcolor rgb \"black\"");
  fprintf(gnuPipe,"\n");
}

void tree_plot(nodo **arbol,FILE* nodos,FILE* aristas,
	       int xmin,int xmax,int ybase){
  int i;
  nodo *n;
  int x,y;
  int xmin2,xmax2,ymin2;
  if(arbol == NULL || *arbol == NULL) return;
  n = *arbol;
  x = (xmin + xmax) / 2;
  y = ybase - HEIGHT;
  fprintf(nodos,"%d %d %d\n",x,y,n->valor);
  if(n->Num_Hijos > 0){
    for(i = 0;i < n->Num_Hijos;i++){
      if((n->hijos)[i] != NULL){
	xmin2 = xmin + i*(xmax - xmin)/(n->Num_Hijos);
	xmax2 = xmin + (i+1)*(xmax - xmin)/(n->Num_Hijos);
	fprintf(aristas,"%d %d %d %d\n",x,y,(xmin2 + xmax2) / 2 - x,-HEIGHT);
	tree_plot(&((n->hijos)[i]),nodos,aristas,xmin2,xmax2,ybase - HEIGHT);
      }
    }
  }
}
