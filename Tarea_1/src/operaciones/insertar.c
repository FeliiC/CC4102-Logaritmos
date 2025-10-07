#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "split.c"
#include "../estructuras/estadisticas.h"

// función para verificar si un nodo esta lleno
int nodo_lleno(Nodo *nodo) {
    return (nodo->k == B);
}

// funcion para insertar un par llave-valor en un arreglo ordenado
void insertar_ordenado(Nodo *nodo, int llave, float valor) {
    if (nodo->k >= B) {
        printf("Error: Nodo lleno, no se puede insertar.\n");
        return;
    }
    int i = nodo->k - 1;
    while (i >= 0 && nodo->llaves_valores[i].llave > llave) {
        nodo->llaves_valores[i+1] = nodo->llaves_valores[i];
        i--;
    }
    nodo->llaves_valores[i+1].llave = llave;
    nodo->llaves_valores[i+1].valor = valor;
    nodo->k++;
}

// funcion para encontrar el hijo apropiado en un nodo interno
int encontrar_hijo(Nodo *nodo, int llave) {
    int i = 0;
    while (i < nodo->k && llave > nodo->llaves_valores[i].llave) {
        i++;
    }
    return i;
}

// funcion para insertar en un arbol B
void insertar_arbolB(Nodo **arbol, int *tamaño_arbol, int indice_nodo, int llave, float valor) {
    
    contar_lectura_insercion(); // lectura
    Nodo *nodo_actual = &arbol[indice_nodo][0];
    
    // si es una hoja
    if (nodo_actual->es_interno == 0) {
        insertar_ordenado(nodo_actual, llave, valor);
        arbol[indice_nodo][0] = *nodo_actual;
        contar_escritura_insercion(); //escritura
        return;
    } 
    // es nodo interno
    int hijo_idx = encontrar_hijo(nodo_actual, llave);
    int indice_hijo = nodo_actual->hijos[hijo_idx];
    contar_lectura_insercion(); // lectura
    if (nodo_lleno(&arbol[indice_hijo][0])) {
        // split del hijo
        SplitArbol split_result = split_arbolB(arbol[indice_hijo][0]);
        int nuevo_indice_der = (*tamaño_arbol)++;
        arbol[nuevo_indice_der] = malloc(sizeof(Nodo));
        *(arbol[nuevo_indice_der]) = split_result.nodo_der;
        contar_escritura_insercion(); // escritura
        *(arbol[indice_hijo]) = split_result.nodo_izq;
        contar_escritura_insercion(); 
        // reorganizar hijos en nodo actual
        for (int i = nodo_actual->k; i > hijo_idx; i--) {
            nodo_actual->hijos[i + 1] = nodo_actual->hijos[i];
        }
        nodo_actual->hijos[hijo_idx + 1] = nuevo_indice_der;
        // insertar par mediano en nodo actual
        insertar_ordenado(nodo_actual, split_result.par_mediano.llave, split_result.par_mediano.valor);
        arbol[indice_nodo][0] = *nodo_actual;
        contar_escritura_insercion();
        
        // insertar recursivamente en el hijo que corresponda
        if (llave <= split_result.par_mediano.llave) {
            insertar_arbolB(arbol, tamaño_arbol, indice_hijo, llave, valor);
        } else {
            insertar_arbolB(arbol, tamaño_arbol, nuevo_indice_der, llave, valor);
        }
    } else {
        // el hijo no esta lleno, insertar recursivamente
        insertar_arbolB(arbol, tamaño_arbol, indice_hijo, llave, valor);
    }
}

// funcion para insertar en un arbol B+
void insertar_arbolBPlus(Nodo **arbol, int *tamaño_arbol, int indice_nodo, int llave, float valor) {
    contar_lectura_insercion();
    Nodo *nodo_actual = &arbol[indice_nodo][0];
    if (nodo_actual->es_interno == 0) { // es hoja entonces inserta directamente
        insertar_ordenado(nodo_actual, llave, valor);
        arbol[indice_nodo][0] = *nodo_actual;
        contar_escritura_insercion();
        return;
    } 

    // es nodo interno
    int hijo_idx = encontrar_hijo(nodo_actual, llave);
    int indice_hijo = nodo_actual->hijos[hijo_idx];
    contar_lectura_insercion();
    if (nodo_lleno(&arbol[indice_hijo][0])) {
        // split del hijo
        SplitArbol split_result;
        int es_hijo_hoja = (arbol[indice_hijo][0].es_interno == 0);
        if (es_hijo_hoja) {
            split_result = split_arbolBPlus(arbol[indice_hijo][0]);
        } else {
            split_result = split_arbolB(arbol[indice_hijo][0]);
        }
        int nuevo_indice_der = (*tamaño_arbol)++;
        arbol[nuevo_indice_der] = malloc(sizeof(Nodo));
        *(arbol[nuevo_indice_der]) = split_result.nodo_der;
        contar_escritura_insercion();
        // arbol B+, si el hijo era hoja se actualiza el puntero siguiente
        if (es_hijo_hoja) {
            split_result.nodo_izq.sgte = nuevo_indice_der;
            split_result.nodo_der.sgte = arbol[indice_hijo][0].sgte;
            // actualizar el nodo derecho
            *(arbol[nuevo_indice_der]) = split_result.nodo_der;
            contar_escritura_insercion();
        }
        *(arbol[indice_hijo]) = split_result.nodo_izq;
        contar_escritura_insercion();

        // reorganizar hijos en nodo actual
        for (int i = nodo_actual->k; i > hijo_idx; i--) {
            nodo_actual->hijos[i + 1] = nodo_actual->hijos[i];
        }
        nodo_actual->hijos[hijo_idx + 1] = nuevo_indice_der;
        
        // insertar par mediano en nodo actual
        insertar_ordenado(nodo_actual, split_result.par_mediano.llave, split_result.par_mediano.valor);
        arbol[indice_nodo][0] = *nodo_actual;
        contar_escritura_insercion();
        
        // insertar recursivamente en el hijo apropiado
        if (llave <= split_result.par_mediano.llave) {
            insertar_arbolBPlus(arbol, tamaño_arbol, indice_hijo, llave, valor);
        } else {
            insertar_arbolBPlus(arbol, tamaño_arbol, nuevo_indice_der, llave, valor);
        }
    } else {
        insertar_arbolBPlus(arbol, tamaño_arbol, indice_hijo, llave, valor);
    }
}

// función para insertar un par en la raiz de un arbol B
void insertar_raiz_arbolB(Nodo ***arbol, int *tamaño_arbol, int *raiz, int llave, float valor) {
    int indice_raiz = *raiz;
    contar_lectura_insercion(); // lectura
    if (!nodo_lleno(&(*arbol)[indice_raiz][0])) {
        insertar_arbolB(*arbol, tamaño_arbol, indice_raiz, llave, valor);
    } else {
        // split de la raiz
        SplitArbol split_result = split_arbolB((*arbol)[indice_raiz][0]);
        int nuevo_indice_izq = (*tamaño_arbol)++;
        (*arbol)[nuevo_indice_izq] = malloc(sizeof(Nodo));
        *((*arbol)[nuevo_indice_izq]) = split_result.nodo_izq;
        contar_escritura_insercion(); // escritura
        
        int nuevo_indice_der = (*tamaño_arbol)++;
        (*arbol)[nuevo_indice_der] = malloc(sizeof(Nodo));
        *((*arbol)[nuevo_indice_der]) = split_result.nodo_der;
        contar_escritura_insercion(); // escritura
        
        // crear nueva raíz
        Nodo *nueva_raiz_nodo = malloc(sizeof(Nodo));
        nueva_raiz_nodo->es_interno = 1;
        nueva_raiz_nodo->k = 1;
        nueva_raiz_nodo->sgte = -1;
        nueva_raiz_nodo->llaves_valores[0] = split_result.par_mediano;
        nueva_raiz_nodo->hijos[0] = nuevo_indice_izq;
        nueva_raiz_nodo->hijos[1] = nuevo_indice_der;
        
        for (int i = 2; i < B+1; i++) {
            nueva_raiz_nodo->hijos[i] = -1;
        }
        
        *((*arbol)[0]) = *nueva_raiz_nodo;
        contar_escritura_insercion(); // escritura
        *raiz = 0;
        free(nueva_raiz_nodo);
        
        // insertar el par original en el hijo apropiado
        if (llave <= split_result.par_mediano.llave) {
            insertar_arbolB(*arbol, tamaño_arbol, nuevo_indice_izq, llave, valor);
        } else {
            insertar_arbolB(*arbol, tamaño_arbol, nuevo_indice_der, llave, valor);
        }
    }
}

// funcion para insertar un par en la raiz de un arbol B+
void insertar_raiz_arbolBPlus(Nodo ***arbol, int *tamaño_arbol, int *raiz, int llave, float valor) {
    int indice_raiz = *raiz;
    
    // LECTURA: Acceso a la raíz
    contar_lectura_insercion();
    
    if (!nodo_lleno(&(*arbol)[indice_raiz][0])) {
        insertar_arbolBPlus(*arbol, tamaño_arbol, indice_raiz, llave, valor);
    } else {
        // split de la raíz
        SplitArbol split_result;
        int es_raiz_anterior_hoja = ((*arbol)[indice_raiz][0].es_interno == 0); 
        if (es_raiz_anterior_hoja) {
            split_result = split_arbolBPlus((*arbol)[indice_raiz][0]);
        } else {
            split_result = split_arbolB((*arbol)[indice_raiz][0]);
        }
        
        int nuevo_indice_der = (*tamaño_arbol)++;
        (*arbol)[nuevo_indice_der] = malloc(sizeof(Nodo));
        *((*arbol)[nuevo_indice_der]) = split_result.nodo_der;
        contar_escritura_insercion(); // escritura

        // arbol B+, si la raiz anterior era hoja se actualizan los punteros
        if (es_raiz_anterior_hoja) {
            split_result.nodo_izq.sgte = nuevo_indice_der;
            split_result.nodo_der.sgte = (*arbol)[indice_raiz][0].sgte;
            // actualizar el nodo derecho con el puntero siguiente
            *((*arbol)[nuevo_indice_der]) = split_result.nodo_der;
            contar_escritura_insercion();
        }
        
        int nuevo_indice_izq = (*tamaño_arbol)++;
        (*arbol)[nuevo_indice_izq] = malloc(sizeof(Nodo));
        *((*arbol)[nuevo_indice_izq]) = split_result.nodo_izq;
        contar_escritura_insercion(); // escritura
        
        // crear nueva raiz
        Nodo *nueva_raiz_nodo = malloc(sizeof(Nodo));
        nueva_raiz_nodo->es_interno = 1;
        nueva_raiz_nodo->k = 1;
        nueva_raiz_nodo->sgte = -1;
        nueva_raiz_nodo->llaves_valores[0] = split_result.par_mediano;
        nueva_raiz_nodo->hijos[0] = nuevo_indice_izq;
        nueva_raiz_nodo->hijos[1] = nuevo_indice_der;
        
        for (int i = 2; i < B+1; i++) {
            nueva_raiz_nodo->hijos[i] = -1;
        }

        *((*arbol)[0]) = *nueva_raiz_nodo;
        contar_escritura_insercion(); // escritura
        *raiz = 0;
        free(nueva_raiz_nodo);
        // insertar el par original en el hijo apropiado
        if (llave <= split_result.par_mediano.llave) {
            insertar_arbolBPlus(*arbol, tamaño_arbol, nuevo_indice_izq, llave, valor);
        } else {
            insertar_arbolBPlus(*arbol, tamaño_arbol, nuevo_indice_der, llave, valor);
        }
    }
}

// funcion principal para insertar en árbol B
void insertar_en_arbolB(Nodo ***arbol, int *tamaño_arbol, int *raiz, int llave, float valor) {
    insertar_raiz_arbolB(arbol, tamaño_arbol, raiz, llave, valor);
}
// funcion principal para insertar en árbol B+
void insertar_en_arbolBPlus(Nodo ***arbol, int *tamaño_arbol, int *raiz, int llave, float valor) {
    insertar_raiz_arbolBPlus(arbol, tamaño_arbol, raiz, llave, valor);
}