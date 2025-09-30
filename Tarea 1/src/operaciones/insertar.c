#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nodo.h"

// función para verificar si un nodo esta lleno
int nodo_lleno(Nodo *nodo) {
    return (nodo->k == 340);
}

// funcion para insertar un par llave-valor en un arreglo ordenado
void insertar_ordenado(Nodo *nodo, int llave, float valor) {
    // verificar que no se exceda de capacidad
    if (nodo->k >= 340) {
        printf("Error: Nodo lleno, no se puede insertar.\n");
        return;
    }
    int i = nodo->k - 1;
    // mover elementos hacia la derecha para hacer espacio
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
    Nodo *nodo_actual = &arbol[indice_nodo][0];
    // si es una hoja
    if (nodo_actual->es_interno == 0) {
        // insertamos el par en el nodo y escribimos el nodo actualizado
        insertar_ordenado(nodo_actual, llave, valor);
        arbol[indice_nodo][0] = *nodo_actual;
    } 
    // es nodo interno
    else {
        // encontrar el hijo apropiado y verificamos si esta lleno
        int hijo_idx = encontrar_hijo(nodo_actual, llave);
        int indice_hijo = nodo_actual->hijos[hijo_idx];
        if (nodo_lleno(&arbol[indice_hijo][0])) {
            // hacemos split y escribimos el nuevo nodo derecho al final del arreglo
            SplitArbol split_result = split_arbolB(arbol[indice_hijo][0]);
            int nuevo_indice_der = (*tamaño_arbol)++;
            arbol[nuevo_indice_der] = malloc(sizeof(Nodo));
            *(arbol[nuevo_indice_der]) = split_result.nodo_der;
            // reemplazamos el nodo hijo izquierdo
            *(arbol[indice_hijo]) = split_result.nodo_izq;
            for (int i = nodo_actual->k; i > hijo_idx; i--) {
                nodo_actual->hijos[i + 1] = nodo_actual->hijos[i];
            }
            // insertamos el indice del nuevo nodo derecho
            nodo_actual->hijos[hijo_idx + 1] = nuevo_indice_der;
            // insertamos el par mediano en el nodo actual y escribirlo
            insertar_ordenado(nodo_actual, split_result.par_mediano.llave, split_result.par_mediano.valor);
            arbol[indice_nodo][0] = *nodo_actual;
            // determinar en cual nodo insertar el par original
            if (llave <= split_result.par_mediano.llave) {
                insertar_arbolB(arbol, tamaño_arbol, indice_hijo, llave, valor);
            } else {
                insertar_arbolB(arbol, tamaño_arbol, nuevo_indice_der, llave, valor);
            }
        } else { // el hijo no esta lleno y se inserta
            insertar_arbolB(arbol, tamaño_arbol, indice_hijo, llave, valor);
        }
    }
}

// funcion para insertar en un arbol B+
void insertar_arbolBPlus(Nodo **arbol, int *tamaño_arbol, int indice_nodo, int llave, float valor) {
    // logica analoga a la de arbol B, pero con consideraciones para B+
    Nodo *nodo_actual = &arbol[indice_nodo][0];
    if (nodo_actual->es_interno == 0) {
        insertar_ordenado(nodo_actual, llave, valor);
        arbol[indice_nodo][0] = *nodo_actual;
    } 
    else {
        int hijo_idx = encontrar_hijo(nodo_actual, llave);
        int indice_hijo = nodo_actual->hijos[hijo_idx];
        if (nodo_lleno(&arbol[indice_hijo][0])) {
            SplitArbol split_result = split_arbolBPlus(arbol[indice_hijo][0]);
            int nuevo_indice_der = (*tamaño_arbol)++;
            arbol[nuevo_indice_der] = malloc(sizeof(Nodo));
            *(arbol[nuevo_indice_der]) = split_result.nodo_der;
            // para el arbol B+, si el nodo dividido era una hoja, actualizar el siguiente
            if (arbol[indice_hijo][0].es_interno == 0) {
                split_result.nodo_izq.sgte = nuevo_indice_der;
            }
            *(arbol[indice_hijo]) = split_result.nodo_izq;
            for (int i = nodo_actual->k; i > hijo_idx; i--) {
                nodo_actual->hijos[i + 1] = nodo_actual->hijos[i];
            }
            nodo_actual->hijos[hijo_idx + 1] = nuevo_indice_der;
            insertar_ordenado(nodo_actual, split_result.par_mediano.llave, split_result.par_mediano.valor);
            arbol[indice_nodo][0] = *nodo_actual;
            if (llave <= split_result.par_mediano.llave) {
                insertar_arbolBPlus(arbol, tamaño_arbol, indice_hijo, llave, valor);
            } else {
                insertar_arbolBPlus(arbol, tamaño_arbol, nuevo_indice_der, llave, valor);
            }
        } else {
            insertar_arbolBPlus(arbol, tamaño_arbol, indice_hijo, llave, valor);
        }
    }
}

// función para insertar un par en la raiz de un arbol B
void insertar_raiz_arbolB(Nodo ***arbol, int *tamaño_arbol, int *raiz, int llave, float valor) {
    int indice_raiz = *raiz;
    // si la raiz no esta llena
    if (!nodo_lleno(&(*arbol)[indice_raiz][0])) {
        insertar_arbolB(*arbol, tamaño_arbol, indice_raiz, llave, valor);
    } else { // hacer split de la raiz
        SplitArbol split_result = split_arbolB((*arbol)[indice_raiz][0]);
        // escribir ambos nuevos nodos al final del arreglo
        int nuevo_indice_izq = (*tamaño_arbol)++;
        int nuevo_indice_der = (*tamaño_arbol)++;
        (*arbol)[nuevo_indice_izq] = malloc(sizeof(Nodo));
        (*arbol)[nuevo_indice_der] = malloc(sizeof(Nodo));
        *((*arbol)[nuevo_indice_izq]) = split_result.nodo_izq;
        *((*arbol)[nuevo_indice_der]) = split_result.nodo_der;
        // crear una nueva raiz vacia
        int nueva_raiz = 0;
        Nodo *nueva_raiz_nodo = malloc(sizeof(Nodo));
        nueva_raiz_nodo->es_interno = 1;
        nueva_raiz_nodo->k = 0;
        nueva_raiz_nodo->sgte = -1;
        for (int i = 0; i < 341; i++) {
            nueva_raiz_nodo->hijos[i] = -1;
        }
        // agregar el par mediano a la lista de pares llave-valor de la nueva raíz
        nueva_raiz_nodo->llaves_valores[0] = split_result.par_mediano;
        nueva_raiz_nodo->k = 1;
        nueva_raiz_nodo->hijos[0] = nuevo_indice_izq;
        nueva_raiz_nodo->hijos[1] = nuevo_indice_der;
        *((*arbol)[nueva_raiz]) = *nueva_raiz_nodo;
        *raiz = nueva_raiz;
        // insertar el nuevo par en el nodo apropiado
        if (llave <= split_result.par_mediano.llave) {
            insertar_arbolB(*arbol, tamaño_arbol, nuevo_indice_izq, llave, valor);
        } else {
            insertar_arbolB(*arbol, tamaño_arbol, nuevo_indice_der, llave, valor);
        }
        free(nueva_raiz_nodo);
    }
}

// funcion para insertar un par en la raiz de un arbol B+
void insertar_raiz_arbolBPlus(Nodo ***arbol, int *tamaño_arbol, int *raiz, int llave, float valor) {
    // logica analoga a insertar_raiz_arbolB, pero con consideraciones para B+
    int indice_raiz = *raiz;
    if (!nodo_lleno(&(*arbol)[indice_raiz][0])) {
        insertar_arbolBPlus(*arbol, tamaño_arbol, indice_raiz, llave, valor);
    } else {
        SplitArbol split_result = split_arbolBPlus((*arbol)[indice_raiz][0]);
        int es_raiz_anterior_hoja = ((*arbol)[indice_raiz][0].es_interno == 0);
        int nuevo_indice_izq = (*tamaño_arbol)++;
        int nuevo_indice_der = (*tamaño_arbol)++;
        (*arbol)[nuevo_indice_izq] = malloc(sizeof(Nodo));
        (*arbol)[nuevo_indice_der] = malloc(sizeof(Nodo));
        *((*arbol)[nuevo_indice_izq]) = split_result.nodo_izq;
        *((*arbol)[nuevo_indice_der]) = split_result.nodo_der;
        if (es_raiz_anterior_hoja) {
            (*arbol)[nuevo_indice_izq]->sgte = nuevo_indice_der;
        }
        int nueva_raiz = 0;
        Nodo *nueva_raiz_nodo = malloc(sizeof(Nodo));
        nueva_raiz_nodo->es_interno = 1;
        nueva_raiz_nodo->k = 0;
        nueva_raiz_nodo->sgte = -1;
        for (int i = 0; i < 341; i++) {
            nueva_raiz_nodo->hijos[i] = -1;
        }
        nueva_raiz_nodo->llaves_valores[0] = split_result.par_mediano;
        nueva_raiz_nodo->k = 1;
        nueva_raiz_nodo->hijos[0] = nuevo_indice_izq;
        nueva_raiz_nodo->hijos[1] = nuevo_indice_der;
        *((*arbol)[nueva_raiz]) = *nueva_raiz_nodo;
        *raiz = nueva_raiz;
        if (llave <= split_result.par_mediano.llave) {
            insertar_arbolBPlus(*arbol, tamaño_arbol, nuevo_indice_izq, llave, valor);
        } else {
            insertar_arbolBPlus(*arbol, tamaño_arbol, nuevo_indice_der, llave, valor);
        }
        
        free(nueva_raiz_nodo);
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