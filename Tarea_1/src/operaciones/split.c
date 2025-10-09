#include <stdio.h>
#include <stdlib.h>
#define B 340

/* Estructura para retornar nodos resultantes de split
    @param nodo_der: Nodo derecho resultante
    @param nodo_izq: Nodo izquierdo resultante
    @param par_mediano: Par mediano  
*/
typedef struct SplitArbol {
    Nodo nodo_der;
    Nodo nodo_izq;
    Pair par_mediano;
} SplitArbol;

/* Guardar pares en un rango especifico en nuevo arreglo
    @param pares: Arreglo de pares originales
    @param nuevos_pares: Arreglo de nuevos pares
    @param init: Indice del primer par a guardar
    @param end: Indice del ultimo par a guardar

    @return Nuevo arreglo de pares en indices [init, end]
*/
Pair *guardar_pares(Pair pares[], Pair nuevos_pares[], int init, int end){  
    int k=0;
    for(int i=init;i<(end);i++){
        nuevos_pares[k]=pares[i];
        k++;
    }
    // Inicializar el resto de pares
    for(int i=k; i<B; i++){
        nuevos_pares[i].llave = 0;
        nuevos_pares[i].valor = 0.0f;
    }
    return nuevos_pares;
}

/* Guardar hijos en un rango especifico en nuevo arreglo
    @param hijos: Arreglo de hijos originales
    @param nuevos_hijos: Arreglo de nuevos hijos
    @param init: Indice del primer hijo a guardar
    @param end: Indice del ultimo hijo a guardar

    @return Nuevo arreglo de hijos en indices [init, end]
*/
int *guardar_hijos(int hijos[], int nuevos_hijos[], int init, int end){
    int k=0;
    for(int i=init;i<(end);i++){
        nuevos_hijos[k]=hijos[i];
        k++;
    }
    // Inicializar el resto de hijos
    for(int i=k; i<B+1; i++){
        nuevos_hijos[i] = -1;
    }
    return nuevos_hijos;
}

/* Realizar split de un nodo
    @param nodo: Nodo a dividir
    @param es_arbolPlus: Indica si es un arbolB+ o no

    @return Los 2 nodos resultantes y el par mediano
*/
SplitArbol split(Nodo n, int es_arbolBPlus){
    //if(nodo_lleno(n)==0) return;
    Pair par_mediano = n.llaves_valores[B/2];

    // Armar nodo derecho
    Nodo nodo_der;
    nodo_der.es_interno = n.es_interno;
    nodo_der.sgte = n.sgte;
    
    if(es_arbolBPlus==1 && n.es_interno==0){
        guardar_pares(n.llaves_valores, nodo_der.llaves_valores, B/2, B);
        nodo_der.k = B - (B/2);
    } else {
        guardar_pares(n.llaves_valores, nodo_der.llaves_valores, B/2 + 1, B);
        nodo_der.k = B - B/2 - 1;
    }
    
    // Armar nodo izquierdo
    Nodo nodo_izq;
    nodo_izq.es_interno = n.es_interno;
    nodo_izq.sgte = n.sgte;
    
    if(es_arbolBPlus==1 && n.es_interno==0){
        guardar_pares(n.llaves_valores, nodo_izq.llaves_valores, 0, B/2 + 1);
        nodo_izq.k = B/2 + 1;
    } else{
        guardar_pares(n.llaves_valores, nodo_izq.llaves_valores, 0, B/2);
        nodo_izq.k = B/2;
    }
    
    // guardar hijos solo si es un nodo interno
    if(n.es_interno){
        guardar_hijos(n.hijos, nodo_izq.hijos, 0, B/2 + 1);         // revisar rangos
        guardar_hijos(n.hijos, nodo_der.hijos, B/2 + 1, B + 1);   // revisar rangos
    } else {
        // Inicializar hijos si es hoja
        for(int i=0; i<B+1; i++){
            nodo_izq.hijos[i] = -1;
            nodo_der.hijos[i] = -1;
        }
    }
    
    // Armar resultado
    SplitArbol result;
    result.nodo_der = nodo_der;
    result.nodo_izq = nodo_izq;
    result.par_mediano = par_mediano; 
    return result; 
}

/* Realizar split en arbolB
    @param nodo: Nodo a dividir
*/
SplitArbol split_arbolB(Nodo n){
    return split(n, 0);
}

/* Realizar split en arbolB+
    @param nodo: Nodo a dividir
*/
SplitArbol split_arbolBPlus(Nodo n){
    return split(n, 1);
}