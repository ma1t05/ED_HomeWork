
/*
  Clase: Estructura de Datos en C++
  22 de Enero de 2015
  Luis Maltos (maltosla@gmail.com)
 */

/* Tarea
   Lograr eliminar elementos del arreglo
   Reducir el tamaño del arreglo
   experimentar, con distintos valores de agregar y reducir
 */

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define AGREGAR 1
#define ELIMINAR 2
#define CONSULTAR 0
#define MAX 101
#define DESDE 10
#define HASTA 99
#define AUSENTE -1
#define DESOCUPADO -1

int arrdin_test(float,float,int);

int main(int argc, char** args){
  float agregar,eliminar;
  int malloc_calls;
  int repeticiones = atoi(args[1]);
  srand(time(NULL));
  FILE * hist = fopen("hist.dat","w");
  printf("agregar\teliminar\tincremento\tdecremento\tmalloc\n");
  fprintf(hist,"elim");
  for(eliminar = 0.05;eliminar < 1.0;eliminar+=0.05){
    fprintf(hist,"\t%.2f",eliminar);
  }
  fprintf(hist,"\n");
  for(agregar = 0.05;agregar < 1.0;agregar+=0.05){
    fprintf(hist,"%.2f",agregar);
    for(eliminar = 0.05;eliminar + agregar < 1.0;eliminar+=0.05){
      printf("%f\t%f\t",agregar,eliminar);
      malloc_calls = arrdin_test(agregar,eliminar,repeticiones);
      printf("%d\n",malloc_calls);
      fprintf(hist,"\t%d",malloc_calls);
    }
    for(;eliminar < 1.0;eliminar+=0.05){
      fprintf(hist,"\t-");
    }
    fprintf(hist,"\n");
  }
  fclose(hist);
}

int arrdin_test(float agregar,float eliminar, int repeticiones){
  
  int *arreglo = NULL, *nuevo;
  int largo = 0;
  int inicial = 1; // Potencia de 2
  int accion = CONSULTAR;
  int s, r;
  int elemento,p,elementos;
  int posicion;
  int malloc_calls = 0;
  int incrementos = 0,decrementos = 0;
  int costly_increases = 0,cheap_increases = 0;
  int costly_decreases = 0,cheap_decreases = 0;
  eliminar = 1.0 - agregar;
  
  for(r = 0;r < repeticiones; r++){
    
    // Determina la accion a realizar
    s = rand() % MAX;
    if(s < agregar * MAX){
      accion = AGREGAR;
    }
    else if (s < (agregar + eliminar) * MAX) {
      accion = ELIMINAR;
    }
    else {
      accion = CONSULTAR;
    }

    elemento = DESDE + rand() % (HASTA - DESDE + 1);  
    switch (accion) {

    case AGREGAR:
      if(largo == 0) {
	//printf("Se inicia el arreglo inicial = %d\n",inicial);
	largo = (1 << inicial);
	arreglo = (int*)malloc(largo * sizeof(int));
	elementos = 0;
	for(p = 0;p < largo;p++)
	  arreglo[p] = DESOCUPADO;
      }
      
      elementos++;
      for(p = 0;p < largo;p++){
	if(arreglo[p] == DESOCUPADO) {
	  arreglo[p] = elemento;
	  posicion = p;
	  break;
	}
      }

      // Doblar tamaño
      if(p == largo){
	//printf("Incrementa el tamaño del arreglo a %d\n",2*largo);
	nuevo = (int*)malloc(2*largo * sizeof(int));
	malloc_calls++;
	incrementos++;
	for(p = 0;p < largo; p++){
	  nuevo[p] = arreglo[p];
	}
	nuevo[largo] = elemento;
	posicion = largo;
	for(p = 1;p < largo; p++){
	  nuevo[largo + p] = DESOCUPADO;
	}
	free(arreglo);
	arreglo = nuevo;
	largo *= 2;
	costly_increases++;
      } else {
	cheap_increases++;
      }
      //printf("A\t%d (%d de %d)\n",elemento,posicion,largo);
      break;

    case ELIMINAR:
      if(elementos > 0) {
	p = rand() % elementos + 1;
	//printf("Borra elemento %d de %d elementos, en la posicion ",p,elementos);
	elementos--;
	for(posicion = 0;posicion < largo;posicion++){
	  if(arreglo[posicion] != DESOCUPADO &&
	     --p == 0) {
	    break;
	  }
	}
	elemento = arreglo[posicion];
	arreglo[posicion] = DESOCUPADO;
	//printf("%d\nE %d @ %d\n",posicion,elemento,posicion);
	
	// Reduce el tamaño
	if(3 * elementos < largo){
	  largo /= 2;
	  //printf("Reduce el tamaño del arreglo a %d\n inicial = %d\n elementos = %d\n",largo,inicial,elementos);
	  if(largo > 0){
	    nuevo = (int*)malloc(largo * sizeof(int));
	    malloc_calls++;
	    decrementos++;
	    for(p = 0,posicion = 0;p < largo; p++){
	      if(arreglo[p] != DESOCUPADO){
		nuevo[posicion++] = arreglo[p];
	      }
	    }
	    for(;posicion < largo; posicion++){
	      nuevo[posicion] = DESOCUPADO;
	    }
	    
	    free(arreglo);
	    arreglo = nuevo;
	  } else {
	    //printf("Se vacia el arreglo\n");
	    free(arreglo);
	    arreglo = NULL;
	  }
	  costly_decreases++;
	} else {
	  cheap_decreases++;
	}
	
      } else {
	//printf("E no hay elementos\n");
      }

      break;

    default: // consultar
      posicion = AUSENTE;
      for(p = 0; p < largo;p++){
	if(arreglo[p] = elemento){
	  posicion = p;
	  break;
	}
      }
      if(posicion = AUSENTE){
	//printf("C %d ausente\n", elemento);
      } else {
	//printf("C %d @ %d\n", elemento, posicion);
      }
      break;
    }
#ifdef DEBUG
    
#endif
  }
  if(arreglo != NULL){
    free(arreglo);
  }
  printf("%d\t%d\t",incrementos,decrementos);
  return malloc_calls;
}
