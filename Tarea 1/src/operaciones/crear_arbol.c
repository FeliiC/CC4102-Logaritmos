#include <stdio.h>
#include <stdlib.h>
#include <nodo.h>

// Inicializar nodo
Nodo inicializar_nodo(int es_interno){
    Nodo n;
    n.es_interno= es_interno; 
    n.k = 0;
    n.sgte = -1;
    return n;
}
