#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {

    long idx = hash(key, map->capacity);

    while ( map->buckets[idx] != NULL && map->buckets[idx]->key != NULL){

        if ( is_equal(key,map->buckets[idx]->key) == 1) return;
        idx = ((idx + 1) % map->capacity);
    }

    if( map->buckets[idx] != NULL){

        map->buckets[idx]->key = key;
        map->buckets[idx]->value = value;

    }else map->buckets[idx] = createPair ( key, value);

    map->size += 1;

}
/*
 Esta función agranda la capacidad del arreglo buckets y reubica todos sus elementos. Para hacerlo es recomendable mantener 
 referenciado el arreglo *actual/antiguo* de la tabla con un puntero auxiliar. Luego, los valores de la tabla se reinicializan 
 con un nuevo arreglo con el **doble de capacidad**. Por último los elementos del arreglo antiguo se insertan en el mapa *vacío* 
 con el método *insertMap*.
Puede seguir los siguientes pasos:

a - Cree una variable auxiliar de tipo Pair** para matener el arreglo map->buckets (*old_buckets*);

b - Duplique el valor de la variable capacity.

c - Asigne a map->buckets un nuevo arreglo con la nueva capacidad.

d - Inicialice size a 0.

e - Inserte los elementos del arreglo *old_buckets* en el mapa (use la función insertMap que ya implementó).

*/
void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)
    Pair ** old_buckets;
    old_buckets = map->buckets;
    long aux;
    aux = map->capacity;
    map->buckets = (Pair**) calloc (map->capacity, sizeof(Pair*));
    map->capacity = aux*2;

    for (long i = 0; i < map->capacity/2; i++){

        if( old_buckets[i] != NULL && old_buckets[i]->key != NULL){
            insertMap(map,old_buckets[i]->key, old_buckets[i]->value);
        }    
    }
    return;

}


HashMap * createMap(long capacity) {

    HashMap *aux;

    aux = (HashMap*) malloc(sizeof(HashMap));
    aux->current= -1;
    aux->buckets = (Pair**)calloc(capacity,sizeof(Pair*));
    aux->capacity = capacity;
    aux->size = 0;
    return aux;
}

void eraseMap(HashMap * map,  char * key) {    

    long idx = hash(key, map->capacity);

    while ( map->buckets[idx] != NULL && map->buckets[idx]->key != NULL){

        if ( is_equal(key,map->buckets[idx]->key) == 1){
            map->current = idx;
            map->buckets[idx]->key = NULL;
            map->size -= 1;
            return;
        }

        idx = (idx + 1) % map->capacity;
 
    } 
    return;

}

Pair * searchMap(HashMap * map,  char * key) {  

    long idx = hash(key, map->capacity);

    while ( map->buckets[idx] != NULL && map->buckets[idx]->key != NULL){

        if ( is_equal(key,map->buckets[idx]->key) == 1){
            map->current = idx;
            return map->buckets[idx];
        }

        idx = (idx + 1) % map->capacity;
        
    } 
    return NULL;
}

Pair * firstMap(HashMap * map) {

    long i;

    for ( i = 0; i < map->capacity; i++){

        if (map->buckets[i] != NULL && map->buckets[i]->key != NULL){
            map->current = i;
            return map->buckets[i];
        }
    }
    return NULL;

}

Pair * nextMap(HashMap * map){

    long i;
    for ( i = map->current+1; i < map->capacity-1; i = ((i + 1) % map->capacity)){
        if (map->buckets[i] != NULL && map->buckets[i]->key != NULL){
            map->current = i;
            return map->buckets[i];
        }
    }
    return NULL;

}
