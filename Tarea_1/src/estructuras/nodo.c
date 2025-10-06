#include <stdio.h>
#include <stdlib.h>
#define B 340

// Estructura pares llave-valor
typedef struct Pair {
    int llave;
    float valor;
} Pair;

// Estructura nodo arbol
typedef struct Nodo {
    int es_interno;
    int k;      // Cantidad de pares llave-valor actualmente contenidas en el nodo (𝑏/2 − 1 ≤ 𝑘 ≤ 𝑏)
    Pair llaves_valores[B];   
    int hijos[B+1]; // guarda las posiciones en disco de los 𝑘 + 1 hijos del nodo
    int sgte;   // Guarda la posición en disco de la hoja siguiente a la actual (se utiliza solo en árbol B+)
} Nodo;