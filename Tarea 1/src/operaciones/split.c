#include <stdio.h>
#include <stdlib.h>
#include <nodo.h>

typedef struct SplitArbol {
    Nodo nodo_der;
    Nodo nodo_izq;
    Pair par_mediano;
} SplitArbol;

Pair *guardar_pares(Pair pares[], Pair nuevos_pares[], int init, int end){  
    int k=0;
    for(int i=init;i<(end);i++){
        nuevos_pares[k]=pares[i];
        k++;
    }
    return nuevos_pares;
}

int *guardar_hijos(int hijos[], int nuevos_hijos[], int init, int end){
    int k=0;
    for(int i=init;i<(end);i++){
        nuevos_hijos[k]=hijos[i];
        k++;
    }
    return nuevos_hijos;
}

SplitArbol split(Nodo n, int is_arbolBPlus){
    //if(nodo_lleno(n)==0) return;
    Pair par_mediano = n.llaves_valores[(340/2)+1];

    // Armar nodo derecho
    Nodo nodo_der;
    nodo_der.es_interno = n.es_interno;
    nodo_der.k = (340/2);
    guardar_pares(n.llaves_valores, nodo_der.llaves_valores, (340/2), 340); // revisar rangos
    nodo_der.sgte = n.sgte;

    // Armar nodo izquierdo
    Nodo nodo_izq;
    nodo_izq.es_interno = n.es_interno;
    if(is_arbolBPlus==0 && n.es_interno==0){
        guardar_pares(n.llaves_valores, nodo_izq.llaves_valores, 0, 340/2);   // revisar rangos
        nodo_izq.k = (340/2);
    } else{
        guardar_pares(n.llaves_valores, nodo_izq.llaves_valores, 0, (340/2 - 1));   // revisar rangos
        nodo_izq.k = (340/2 -1);
    }
    nodo_izq.sgte = n.sgte;
    // guardar hijos solo si es un nodo interno
    if(n.es_interno){
        guardar_hijos(n.hijos, nodo_izq.hijos, 0, (340/2));         // revisar rangos
        guardar_hijos(n.hijos, nodo_izq.hijos, (340/2 + 1), 340);   // revisar rangos
    } 
    // Armar resultado
    SplitArbol result;
    result.nodo_der = nodo_der;
    result.nodo_izq = nodo_izq;
    result.par_mediano = par_mediano;

    return result; 
}

SplitArbol split_arbolB(Nodo n){
    return split(n, 1);
}

SplitArbol split_arbolBPlus(Nodo n){
    return split(n, 0);
}