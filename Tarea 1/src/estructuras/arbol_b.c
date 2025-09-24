#include <stdio.h>
#include <stdlib.h>

int b = 340;

typedef struct Pair {
    int llave;
    float valor;
} Pair;

// Estructura arbol B
typedef struct ArbolB {
    int es_interno;
    int k;      // Cantidad de pares llave-valor actualmente contenidas en el nodo
    Pair llaves_valores[340]; 
    int hijos[341];
    int sgte;
} ArbolB;