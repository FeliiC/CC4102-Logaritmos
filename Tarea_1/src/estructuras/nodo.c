#include <stdio.h>
#include <stdlib.h>
#define B 340

/* Estructura pares llave-valor
    @param llave: tiempo(segundo) en que se realizo la medicion
    @param valor: temperatura medida
*/
typedef struct Pair {
    int llave;
    float valor;
} Pair;

/* Estructura nodo arbol
    @param es_interno: Indica si es un nodo interno o una hoja
    @param k: Cantidad de pares llave-valor actualmente contenidas en el nodo (𝑏/2 − 1 ≤ 𝑘 ≤ 𝑏)
    @param llaves_valores: Guarda los pares llave-valor
    @param hijos: Guarda las posiciones en disco de los 𝑘 + 1 hijos del nodo
    @param sgte: Guarda la posición en disco de la hoja siguiente a la actual (se utiliza solo en árbol B+) 
*/
typedef struct Nodo {
    int es_interno;
    int k;                      
    Pair llaves_valores[B];   
    int hijos[B+1];             
    int sgte;                   
} Nodo;

/* Estructura de cantidad de IOs
    @param lecturas_busqueda: Cantidad de lecturas (en IOs) al buscar
    @param escrituras_insercion: Cantidad de escrituras (en IOs) al insertar
    @param lecturas_insercion: Cantidad de lecturas (en IOs) al insertar
    @param total_operaciones: Cantidad total de IOs 
*/
typedef struct StatsIO {
    int lecturas_busqueda;
    int escrituras_insercion;
    int lecturas_insercion;
    int total_operaciones;
} StatsIO;