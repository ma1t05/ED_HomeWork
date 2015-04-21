 
#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#define MAX 10

/*
  Determinar la complejidad de mi algoritmo 
*/

struct elemento{
  int valor;
  struct elemento* sig;
  struct elemento* ant;
};


struct listaEnlazada{
  struct elemento* inicio;
  struct elemento* final;
};

typedef struct elemento elem;
typedef struct listaEnlazada lista;


void imprimeTodo(lista*);
void imprime_arreglo(int*,int);
void imprimeAlrevez(lista*);
void agregarAlInicio(lista*,elem*);
void agregarAlFinal(lista*,elem*);
void agregaOrdenado(lista*,elem*);

void free_list(lista*);

int es_elemento(lista*,int);
// 0 no es elemento, 1 si lo es
int es_elemento_ord(lista*,int);
// igual que es_elemento con lista ordenada
int posicion_elemento(lista*,int);
// -1 si no es elemento, 0 a n-1 indicando la posicion
int posicion_elemento_ord(lista*,int);
// como posicion_elemento ordenada
void inserta_despues_de_valor(lista*,int,elem*);
// Inserta despues del primer valor encontrado
void inserta_despues_de_elemento(lista*,elem*,elem*); 
// Inserta despues de elemento, verifica pertenencia
void _inserta_despues_de_elemento(lista*,elem*,elem*); 
// Inserta despues de elemento
void inserta_antes_de_valor(lista*,int,elem*); 
// Inserta antes del primer valor encontrado
void inserta_antes_de_elemento(lista*,elem*,elem*);
// Inserta antes del elemento, verifica pertenencia
void _inserta_antes_de_elemento(lista*,elem*,elem*);
// Inserta antes del elemento
elem* remueveInicio(lista*);
// Regresa el primer elemento o null, despues de removerlo
elem* remueveFinal(lista*);
// Regresa el ultimo elemento o null, despues de removerlo
void remueve_valor_de(lista*,int);
// Remueve el primer valor encontrado
void remueve_valores_de(lista*,int);
// Remueve todos los valores encontrados
void _remueve_elemento(lista*,elem*);
// Remueve elemento de lista
void remueve_rango(lista*,int,int);
//void remueve_elemento(lista*,elem*);
lista* junta_listas(lista*,lista*);
// une dos listas
lista* _junta_listas(lista*,lista*);
// une dos listas
lista* junta_listas_ord(lista*,lista*);
// une dos listas ordenadas, manteniendo el orden
lista* crea_lista(int);
int * crea_arreglo(int);

int maltos_sort(lista*);
lista* incercion_sort(lista*);
int quick_sort(lista*);
int merge_sort(lista*);
int bubble_sort(int*,int);
int seleccion_sort(int*,int);

int main(int argc, char** argv){
  int i;
  int *a = NULL;
  lista* l = NULL;
  lista* lo = NULL;
  elem* e = NULL;
  elem* t = NULL;
  int v;

  l = (lista*)malloc(sizeof(lista));
  l->inicio = NULL;
  l->final = NULL;
  lo = (lista*)malloc(sizeof(lista));
  lo->inicio = NULL;
  lo->final = NULL;

  srand(time(NULL));

  for(i = 0;i < 10;i++){
    e = (elem*)malloc(sizeof(elem));
    e->valor = rand()%MAX + 1;
    if(i%2 == 0){
      agregarAlInicio(l,e);
    } else {
      agregarAlFinal(l,e);
    }

    t = (elem*)malloc(sizeof(elem));
    t->valor = e->valor;
    agregaOrdenado(lo,t);

    imprimeTodo(l);
    imprimeTodo(lo);
  }
  imprimeAlrevez(lo);

  // Prueba es elemento posicion sin ordenar y ordenado
  for(i = 0;i < 10;i++){
    v = rand()%MAX + 1;
    printf("El %d es elemento de la lista?\n",v);
    if(es_elemento(l,v) > 0) {
      printf("Si en la posicion %d sin ordenar y %d ordenado\n",posicion_elemento(l,v),posicion_elemento_ord(lo,v));
    } else {
      printf("No\n");
    }
  }
  free_list(lo);

  // Prueba insertar antes/despues de valor
  for(i = 0;i < 10;i++){
    e = (elem*)malloc(sizeof(elem));
    e->valor = rand()%MAX + 1;
    do{
      v = rand()%MAX + 1;
      if(i%2){
	printf("Inserta %d despues del %d\n",e->valor,v);
	inserta_despues_de_valor(l,v,e);
      } else {
	printf("Inserta %d antes del %d\n",e->valor,v);
	inserta_antes_de_valor(l,v,e);
      }
    } while(!es_elemento(l,v));
    printf("Se inserto!\n");
    imprimeTodo(l);
  }

  // Prueba remueve inicio/final
  for(i = 0;i < 10;i++){
    if(i%2)
      e = remueveInicio(l);
    else
      e = remueveFinal(l);

    if(e != NULL){
      printf("Se removio el %d\n",e->valor);
      free(e);
    }
    imprimeTodo(l);
  }

  // Prueba remover elemento(s)
  for(i = 0;i < 10;i++){
    v = rand() % MAX;

    if(i%2){
      printf("Se queriere remover el %d\n",v);
      remueve_valor_de(l,v);
    } else {
      printf("Se quieren remiover los valores %d\n",v);
      remueve_valores_de(l,v);
    }

    imprimeTodo(l);
  }
  /*
  // Prueba Sort
  printf("Prueba Sort\n");
  imprimeTodo(l);
  maltos_sort(l);
  imprimeTodo(l);

  // vaciar todo
  printf("Vacia listas\n");
  imprimeTodo(l);
  free_list(l);
  printf("Termina Vaciar listas\n");
  */
  lista *ll;
  l = crea_lista(10);
  imprimeTodo(l);
  lo = crea_lista(10);
  imprimeTodo(lo);
  ll = junta_listas(l,lo);
  imprimeTodo(ll);
  free_list(ll);
  maltos_sort(l);
  maltos_sort(lo);
  ll = junta_listas_ord(l,lo);
  imprimeTodo(ll);
  free_list(l);
  free_list(lo);
  free_list(ll);

  // Prueba incerción sort
  printf("Prueba ordenamiento por incerción\n");
  l = crea_lista(10);
  imprimeTodo(l);
  lo = incercion_sort(l);
  imprimeTodo(lo);
  free_list(l);
  free_list(lo);
  
  // Prueba quick sort
  printf("Prueba ordenamiento rapido\n");
  l = crea_lista(10);
  imprimeTodo(l);
  quick_sort(l);
  imprimeTodo(l);
  free_list(l);
  
  // Prueba merge sort
  printf("Prueba ordenamiento merge\n");
  l = crea_lista(10);
  imprimeTodo(l);
  merge_sort(l);
  imprimeTodo(l);
  free_list(l);
  
  // Prueba bubble sort
  printf("Pureba ordenamiento burbuja\n");
  a = crea_arreglo(10);
  imprime_arreglo(a,10);
  bubble_sort(a,10);
  imprime_arreglo(a,10);
  free(a);

  // Prueba seleccion sort
  printf("Prueba ordenamiento por seleccion\n");
  a = crea_arreglo(10);
  imprime_arreglo(a,10);
  bubble_sort(a,10);
  imprime_arreglo(a,10);
  free(a);

  return 0;
}


void imprimeTodo(lista* cual){
  elem* actual = cual->inicio;
  printf("[");
  while(actual != NULL){
    printf(" %d",actual->valor);
    actual = actual->sig;
  }
  printf(" ]\n");
}

void imprime_arreglo(int *a,int n){
  int i;
  printf("[");
  for(i = 0;i < n;i++) printf("%d ",a[i]);
  printf("]\n");
}

void imprimeAlrevez(lista* cual){
  elem* actual = cual->final;
  printf("[");
  while(actual != NULL){
    printf(" %d",actual->valor);
    actual = actual->ant;
  }
  printf(" ]\n");
}

void agregarAlInicio(lista* alCual,elem* queCosa){
  if(alCual->inicio == NULL){
    alCual->inicio = queCosa;
    alCual->final = queCosa;
    queCosa->sig = NULL;
    queCosa->ant = NULL;
  } else {
    alCual->inicio->ant = queCosa;
    queCosa->sig = alCual->inicio;
    queCosa->ant = NULL;
    alCual->inicio = queCosa;
  }
}

void agregarAlFinal(lista* alCual,elem* queCosa){
  if(alCual->final == NULL){
    alCual->inicio = queCosa;
    alCual->final = queCosa;
    queCosa->sig = NULL;
    queCosa->ant = NULL;
  } else {
    queCosa->sig = NULL;
    queCosa->ant = alCual->final;
    alCual->final->sig = queCosa;
    alCual->final = queCosa;
  }
}

int agregaOrdenado(lista* alCual,elem* queCosa){
  int k = 0;
  if(alCual->inicio == NULL){
    alCual->inicio = queCosa;
    alCual->final = queCosa;
    queCosa->ant = NULL;
    queCosa->sig = NULL;
    k = 1;
  } else {
    elem* t = alCual->inicio;
    elem* p = NULL;
    while(t != NULL && t->valor < queCosa->valor){
      k++;
      p = t;
      t = t->sig;
    }
    queCosa->sig = t;
    queCosa->ant = p;
    if(p == NULL) {
      alCual->inicio = queCosa;
    } else {
      p->sig = queCosa;
    }
    if(t != NULL){
      t->ant = queCosa;
    } else {
      alCual->final = queCosa;
    }
  }
  return k;
}

void free_list(lista *l){
  elem *e = l->inicio, *t;
  while(e != NULL){
    t = e->sig;
    free(e);
    e = t->sig;
  }
  free(l);
}

int es_elemento(lista* l,int i){
  elem* e = l->inicio;
  while(e != NULL){
    if(e->valor == i)
      return 1;
    e = e->sig;
  }
  return 0;
}

int es_elemento_ord(lista* l,int i){
  elem* e = l->inicio;
  while(e != NULL && e->valor < i){
    if(e->valor == i)
      return 1;
    e = e->sig;
  }
  return 0;
}

int posicion_elemento(lista* l ,int i){
  int k = 0;
  elem* e = l->inicio;
  while(e != NULL){
    if(e->valor == i)
      return k;
    e = e->sig;
    k++;
  }
  return -1;
}

int posicion_elemento_ord(lista* l,int i){
  int k = 0;
  elem* e = l->inicio;
  while(e != NULL && e->valor < i){
    e = e->sig;
    k++;
  }
  return (e != NULL) ? k : -1;
}

void inserta_despues_de_valor(lista* l,int i,elem* a_insertar){
  elem *t = l->inicio;
  while(t != NULL){
    if(t->valor == i) {
      _inserta_despues_de_elemento(l,t,a_insertar);
      break;
    }
    t = t->sig;
  }
}

void inserta_despues_de_elemento(lista* l,elem* t,elem* a_insertar){
  elem *s = l->inicio;
  while(s != NULL){
    if(s == t) {
      _inserta_despues_de_elemento(l,t,a_insertar);
      break;
    }
    s = s->sig;
  }
}

void _inserta_despues_de_elemento(lista* l,elem* referencia,elem* a_insertar){
  a_insertar->sig = referencia->sig;
  a_insertar->ant = referencia;
  referencia->sig = a_insertar;
  if(a_insertar->sig != NULL)
    a_insertar->sig->ant = a_insertar;
  else
    l->final = a_insertar;
    
}

void inserta_antes_de_valor(lista* l,int i,elem* a_insertar){
  elem* t = l->inicio;
  while(t != NULL){
    if(t-> valor == i){
      _inserta_antes_de_elemento(l,t,a_insertar);
      break;
    }
    t = t->sig;
  }
}

void inserta_antes_de_elemento(lista* l,elem* t,elem* a_insertar){
  elem* s = l->inicio;
  while(s != NULL){
    if(s == t){
      _inserta_antes_de_elemento(l,t,a_insertar);
      break;
    }
    s = s->sig;
  }
}

void _inserta_antes_de_elemento(lista* l,elem* t,elem* a_insertar){
  a_insertar->sig = t;
  a_insertar->ant = t->ant;
  t->ant = a_insertar;
  if(a_insertar->ant != NULL)
    a_insertar->ant->sig = a_insertar;
  else
    l->inicio = a_insertar;
}

elem* remueveInicio(lista* l){
  elem* e = l->inicio;
  if(e != NULL){
    l->inicio = e->sig;
    if(e->sig != NULL){
      e->sig->ant = NULL;
      e->sig = NULL;
    } else {
      l->final = NULL;
    }
  }
  return e;
}

elem* remueveFinal(lista* l){
  elem* e = l->final;
  if(e != NULL){
    l->final = e->ant;
    if(e->ant != NULL){
      e->ant->sig = NULL;
      e->ant = NULL;
    } else {
      l->inicio = NULL;
    }
  }
  return e;
}

void remueve_valor_de(lista* l,int i){
  elem* e = l->inicio;
  while(e != NULL && e->valor != i) e = e->sig;
  if(e != NULL){
    _remueve_elemento(l,e);
    free(e);
  }
}

void remueve_valores_de(lista* l ,int i){
  elem* e = l->inicio, *t;
  while(e != NULL){ 
    t = e->sig;
    if(e->valor == i){
      _remueve_elemento(l,e);
      printf("Removio elemento %d\n",i);
      free(e);
    }
    e = t;
  }
}

void _remueve_elemento(lista* l,elem* e){
  elem* t;
  if(e->ant != NULL){
    e->ant->sig = e->sig;
  } else {
    l->inicio = e->sig;
  }
  
  if(e->sig != NULL){
    e->sig->ant = e->ant;
  } else {
    l->final = e->ant;
  }

  e->ant = NULL;
  e->sig = NULL;
}

void remueve_rango(lista* l,int i,int j){
  elem* e = l->inicio,*t,*s;
  int k = 0;
  while(e != NULL && k < i){
    e = e->sig;
    k++;
  }
  if(e != NULL){
    t = e->ant;
    for(;i < j;i++){
      s = e->sig;
      free(e);
      e = s;
      if(e == NULL)
	break;
    }
    if(e != NULL){ 
      e->ant = t;
      if(t != NULL)
	t->sig = e;
      else
	l->inicio = e;
    } else {
      l->final = t;
      if(t != NULL)
	t->sig = NULL;
      else
	l->inicio = NULL;
    }
  }
}

int maltos_sort(lista* l){
  elem *e,*t,*p,*s;
  int k = 0;
  p = l->inicio;
  while(p != NULL){
    e = p->sig;
    while(e != NULL){
      s = e->sig;
      _remueve_elemento(l,e); /* */ k++;
      if(e->valor < p->valor){
	t = p->ant;
	while(t != NULL && t->valor > e->valor){
	  t = t->ant; /* */ k++;
	}
	if(t != NULL){
	  _inserta_despues_de_elemento(l,t,e);
	} else {
	  agregarAlInicio(l,e);
	}
      } else {
	t = p->sig;
	while(t != NULL && t->valor < e->valor) t = t->sig;
	if(t != NULL){
	  _inserta_antes_de_elemento(l,t,e);
	} else {
	  agregarAlFinal(l,e);
	}
      }
      e = s;
    }
    p = p->sig;
  }
  return k;
}

lista* junta_listas(lista* lst_1,lista* lst_2){
  lista *lst;
  elem *e,*s;
  lst = (lista*)malloc(sizeof(lista));
  lst->inicio = NULL;
  lst->final = NULL;
  e = lst_1->inicio;
  while(e != NULL){
    s = (elem*)malloc(sizeof(elem));
    s->valor = e->valor;
    agregarAlFinal(lst,s);
    e = e->sig;
  }
  e = lst_2->inicio;
  while(e != NULL){
    s = (elem*)malloc(sizeof(elem));
    s->valor = e->valor;
    agregarAlFinal(lst,s);
    e = e->sig;
  }
  return lst;
}

lista* _junta_listas(lista *lst_1,lista *lst_2){
  lista *lst;
  elem *e,*t;
  lst = (lista*)malloc(sizeof(lista));
  lst->inicio = NULL;
  lst->final = NULL;
  e = lst_1->inicio;
  while(e != NULL){
    t = e->sig;
    _remueve_elemento(lst_1,e);
    agregarAlFinal(lst,e);
    e = t;
  }
  e = lst_2->inicio;
  while(e != NULL){
    t = e->sig;
    _remueve_elemento(lst_2,e);
    agregarAlFinal(lst,e);
    e = t;
  }
  free_list(lst_1);
  free_list(lst_2);
  return lst;
}


lista* junta_listas_ord(lista *lst_1,lista *lst_2){
  lista* lst;
  elem *e,*t,*s,*r;
  lst = (lista*)malloc(sizeof(lista));
  lst->inicio = NULL;
  lst->final = NULL;
  e = lst_1->inicio;
  t = lst_2->inicio;
  while(e != NULL && t != NULL){
    if(e->valor < t->valor){
      s = e->sig;
      r = (elem*)malloc(sizeof(elem));
      r->valor = e->valor;
      agregarAlFinal(lst,r);
      e = s;
    } else {
      s = t->sig;
      r = (elem*)malloc(sizeof(elem));
      r->valor = t->valor;
      agregarAlFinal(lst,r);
      t = s;
    }
  }
  while(e != NULL){
    s = e->sig;
    r = (elem*)malloc(sizeof(elem));
    r->valor = e->valor;
    agregarAlFinal(lst,r);
    e = s;
  }
  while(t != NULL){
    s = t->sig;
    r = (elem*)malloc(sizeof(elem));
    r->valor = t->valor;
    agregarAlFinal(lst,r);
    t = s;
  }
  return lst;
  
}

lista* _junta_listas_ord(lista *lst_1,lista *lst_2){
  lista* lst;
  elem *e,*t,*s;
  lst = (lista*)malloc(sizeof(lista));
  lst->inicio = NULL;
  lst->final = NULL;
  e = lst_1->inicio;
  t = lst_2->inicio;
  while(e != NULL && t != NULL){
    if(e->valor < t->valor){
      s = e->sig;
      _remueve_elemento(lst_1,e);
      agregarAlFinal(lst,e);
      e = s;
    } else {
      s = t->sig;
      _remueve_elemento(lst_2,t);
      agregarAlFinal(lst,t);
      t = s;
    }
  }
  while(e != NULL){
    s = e->sig;
    _remueve_elemento(lst_1,e);
    agregarAlFinal(lst,e);
    e = s;
  }
  while(t != NULL){
    s = t->sig;
    _remueve_elemento(lst_2,t);
    agregarAlFinal(lst,t);
    t = s;
  }
  return lst;
}

lista* crea_lista(int size){
  lista *l;
  elem *e;
  l = (lista*)malloc(sizeof(lista));
  l->inicio = NULL;
  l->final = NULL;
  while(size-->0){
    e = (elem*)malloc(sizeof(elem));
    e->valor = rand() % MAX;
    agregarAlFinal(l,e);
  }
  return l;
}

int* crea_arreglo(int size){
  int *a = NULL;
  a = (int*)malloc(sizeof(int)*size);
  while(size-->0){
    a[size] = rand() % MAX;
  }
  return a;
}

lista* incercion_sort(lista* lst){
  lista* ll;
  elem *e,*s;
  ll = (lista*)malloc(sizeof(lista));
  ll->inicio = NULL;
  ll->final = NULL;
  e = lst->inicio;
  while(e != NULL){
    s = (elem*)malloc(sizeof(elem));
    s->valor = e->valor;
    agregaOrdenado(ll,s);
    e = e->sig;
  }
  return ll;
}

int quick_sort(lista* lst){
  lista *lst_izq,*lst_der; 
  elem *piv,*e,*s;
  int altura_izq,altura_der;
  if(lst->inicio == lst->final) return 1;
  lst_izq = (lista*)malloc(sizeof(lista));
  lst_der = (lista*)malloc(sizeof(lista));
  lst_izq->inicio = NULL;
  lst_izq->final = NULL;
  lst_der->inicio = NULL;
  lst_der->final = NULL;

  // Selecciona Pivote
  piv = lst->inicio;
  _remueve_elemento(lst,piv);

  e = lst->inicio;
  while(e != NULL){
    s = e->sig;
    _remueve_elemento(lst,e);
    if(e->valor < piv->valor)
      agregarAlFinal(lst_izq,e);
    else
      agregarAlFinal(lst_der,e);
    e = s;
  }
  if(lst_izq->inicio == NULL)
    agregarAlFinal(lst_izq,piv);
  else
    agregarAlFinal(lst_der,piv);

  altura_izq = quick_sort(lst_izq);
  altura_der = quick_sort(lst_der);
  
  lst->inicio = lst_izq->inicio;
  lst_izq->final->sig = lst_der->inicio;
  lst->final = lst_der->final;
  
  lst_izq->inicio = NULL;
  lst_izq->final = NULL;
  free(lst_izq);
  lst_der->inicio = NULL;
  lst_der->final = NULL;
  free(lst_der);

  return (altura_izq > altura_der ? altura_izq : altura_der) + 1;

}

int merge_sort(lista *lst){
  lista *lst_izq,*lst_der,*lst_aux;
  elem *e,*s;
  int altura_izq,altura_der,k;
  if(lst->inicio == lst->final) return 1;
  
  lst_izq = (lista*)malloc(sizeof(lista));
  lst_der = (lista*)malloc(sizeof(lista));
  lst_izq->inicio = lst->inicio;
  lst_der->final = lst->final;
  e = lst->inicio;
  k = 0;
  while(e != NULL) {
    k++;
    e = e->sig;
  }
  k = (k%2 ? k : (k - 1)) >> 1;
  e = lst->inicio;
  for(;k>0;k--) e = e->sig;
  s = e->sig;
  lst_izq->final = e;
  e->sig = NULL;
  lst_der->inicio = s;
  s->ant = NULL;

  lst->inicio = NULL;
  lst->final = NULL;

  altura_izq = merge_sort(lst_izq);
  altura_der = merge_sort(lst_der);
  
  lst_aux = _junta_listas_ord(lst_izq,lst_der);
  free(lst_izq);
  free(lst_der);

  lst->inicio = lst_aux->inicio;
  lst->final = lst_aux->final;
  
  lst_aux->inicio = NULL;
  lst_aux->final = NULL;
  free(lst_aux);
  
  return (altura_izq > altura_der ? altura_izq : altura_der) + 1;
}

int bubble_sort(int* a,int n){
  int i,c;
  int t;
  do {
    c = 0;
    for(i = 1;i<n;i++){
      if(a[i]<a[i-1]){
	t = a[i];
	a[i] = a[i-1];
	a[i-1] = t;
	c++;
      }
    }
  }while(c != 0);
  return a[0];
}

int seleccion_sort(int *a,int n){
  int i,j,k;
  int t;
  for(i = 0;i< n;i++){
    k = i;
    for(j = i + 1;j < n;j++){
      if(a[j] < a[k]){
	k = j;
      }
    }
    t = a[k];
    a[k] = a[i];
    a[i] = t;
  }
}
