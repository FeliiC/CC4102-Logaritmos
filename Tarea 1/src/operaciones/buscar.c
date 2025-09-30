#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nodo.h"

// funcion para buscar un rango de llaves en el archivo binario
void buscar_rango_desde_archivo(const char *nombre_archivo, int limite_inf, int limite_sup) {
    FILE *archivo = fopen(nombre_archivo, "rb");
    if (archivo == NULL) {
        printf("Error al abrir el archivo: %s\n", nombre_archivo);
        return;
    }
    // calcular el tamaño del archivo
    fseek(archivo, 0, SEEK_END);
    long tamano_archivo = ftell(archivo);
    rewind(archivo);
    int tamaño_nodo = sizeof(Nodo);
    int num_nodos = tamano_archivo / tamaño_nodo;
    int indice_actual = 0;
    Nodo nodo_actual;
    // leer la raiz y buscar
    fseek(archivo, indice_actual * tamaño_nodo, SEEK_SET);
    fread(&nodo_actual, tamaño_nodo, 1, archivo);
    buscar_rango_recursivo(archivo, &nodo_actual, indice_actual, limite_inf, limite_sup, tamaño_nodo);
    fclose(archivo);
}

void buscar_rango_recursivo(FILE *archivo, Nodo *nodo, int indice_actual, int limite_inf, int limite_sup, int tamaño_nodo) {
    if (nodo->es_interno == 0) {
        // si es hoja, buscar los pares en el rango
        for (int i = 0; i < nodo->k; i++) {
            if (nodo->llaves_valores[i].llave >= limite_inf && nodo->llaves_valores[i].llave <= limite_sup) {
                printf("Llave: %d, Valor: %.2f\n", nodo->llaves_valores[i].llave, nodo->llaves_valores[i].valor);
            }
        }
        // para arbol B+, si hay siguiente hoja, continuar buscando
        if (nodo->sgte != -1) {
            Nodo siguiente;
            fseek(archivo, nodo->sgte * tamaño_nodo, SEEK_SET);
            fread(&siguiente, tamaño_nodo, 1, archivo);
            buscar_rango_recursivo(archivo, &siguiente, nodo->sgte, limite_inf, limite_sup, tamaño_nodo);
        }
    } else { // es nodo interno
        int i = 0;
        while (i < nodo->k && nodo->llaves_valores[i].llave < limite_inf) {
            i++;
        }
        // buscar en el hijo correspondiente
        Nodo hijo;
        fseek(archivo, nodo->hijos[i] * tamaño_nodo, SEEK_SET);
        fread(&hijo, tamaño_nodo, 1, archivo);
        buscar_rango_recursivo(archivo, &hijo, nodo->hijos[i], limite_inf, limite_sup, tamaño_nodo);
        // para el arbol B+, tambien buscar en hijos siguientes si las llaves estan en rango
        i++;
        while (i <= nodo->k && nodo->llaves_valores[i-1].llave <= limite_sup) {
            Nodo hijo_sig;
            fseek(archivo, nodo->hijos[i] * tamaño_nodo, SEEK_SET);
            fread(&hijo_sig, tamaño_nodo, 1, archivo);
            buscar_rango_recursivo(archivo, &hijo_sig, nodo->hijos[i], limite_inf, limite_sup, tamaño_nodo);
            i++;
        }
    }
}