#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void buscar_rango_recursivo(FILE *archivo, Nodo *nodo, int indice_actual, int limite_inf, int limite_sup, int tamano_nodo, Pair** resultados, int* cantidad);

// funcion para buscar un rango de llaves en el archivo binario
Pair* buscar_rango_desde_archivo(const char *nombre_archivo, int limite_inf, int limite_sup, int* cantidad_resultados) {
    Pair* resultados = malloc(sizeof(Pair) * 1000);
    int cantidad = 0;
    FILE *archivo = fopen(nombre_archivo, "rb");
    if (archivo == NULL) {
        printf("Error al abrir el archivo: %s\n", nombre_archivo);
        free(resultados);
        return resultados;
    }
    // calcular el tamano del archivo
    fseek(archivo, 0, SEEK_END);
    long tamano_archivo = ftell(archivo);
    rewind(archivo);
    int tamano_nodo = sizeof(Nodo);
    int num_nodos = tamano_archivo / tamano_nodo;
    int indice_actual = 0;
    Nodo nodo_actual;
    // leer la raiz y buscar
    fseek(archivo, indice_actual * tamano_nodo, SEEK_SET);
    fread(&nodo_actual, tamano_nodo, 1, archivo);
    buscar_rango_recursivo(archivo, &nodo_actual, indice_actual, limite_inf, limite_sup, tamano_nodo, &resultados, &cantidad);
    fclose(archivo);
    *cantidad_resultados = cantidad;
    return resultados;
}

void buscar_rango_recursivo(FILE *archivo, Nodo *nodo, int indice_actual, int limite_inf, int limite_sup, int tamano_nodo, Pair** resultados, int* cantidad) {
    if (nodo->es_interno == 0) {
        // si es hoja, buscar los pares en el rango
        for (int i = 0; i < nodo->k; i++) {
            if (nodo->llaves_valores[i].llave >= limite_inf && nodo->llaves_valores[i].llave <= limite_sup) {
                // printf("Llave: %d, Valor: %.2f\n", nodo->llaves_valores[i].llave, nodo->llaves_valores[i].valor);
                (*resultados)[*cantidad] = nodo->llaves_valores[i];
                (*cantidad)++;
                if (*cantidad==1000) printf("Se alcanzo el limite de pares encontrados");
            }
        }
        // para arbol B+, si hay siguiente hoja, continuar buscando
        if (nodo->sgte != -1) {
            Nodo siguiente;
            fseek(archivo, nodo->sgte * tamano_nodo, SEEK_SET);
            fread(&siguiente, tamano_nodo, 1, archivo);
            buscar_rango_recursivo(archivo, &siguiente, nodo->sgte, limite_inf, limite_sup, tamano_nodo, resultados, cantidad);
        }
    } else { // es nodo interno
        int i = 0;
        while (i < nodo->k && nodo->llaves_valores[i].llave < limite_inf) {
            i++;
        }
        // buscar en el hijo correspondiente
        Nodo hijo;
        fseek(archivo, nodo->hijos[i] * tamano_nodo, SEEK_SET);
        fread(&hijo, tamano_nodo, 1, archivo);
        buscar_rango_recursivo(archivo, &hijo, nodo->hijos[i], limite_inf, limite_sup, tamano_nodo, resultados, cantidad);
        // para el arbol B+, tambien buscar en hijos siguientes si las llaves estan en rango
        i++;
        while (i <= nodo->k && nodo->llaves_valores[i-1].llave <= limite_sup) {
            Nodo hijo_sig;
            fseek(archivo, nodo->hijos[i] * tamano_nodo, SEEK_SET);
            fread(&hijo_sig, tamano_nodo, 1, archivo);
            buscar_rango_recursivo(archivo, &hijo_sig, nodo->hijos[i], limite_inf, limite_sup, tamano_nodo, resultados, cantidad);
            i++;
        }
    }
}