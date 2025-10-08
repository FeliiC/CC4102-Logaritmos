#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void contar_lectura_busqueda(StatsIO* stats_global);

// funcion auxiliar para buscar en arbol B recursiva
void buscar_rango_arbolB_recursivo(FILE *archivo, int indice_actual, int limite_inf, int limite_sup, int tamano_nodo, Pair** resultados, int* cantidad, int* capacidad, StatsIO* stats) {
    Nodo nodo_actual;
    fseek(archivo, indice_actual * tamano_nodo, SEEK_SET);
    fread(&nodo_actual, tamano_nodo, 1, archivo);
    contar_lectura_busqueda(stats);
    
    if (nodo_actual.es_interno == 0) {
        // revisar todos los pares en el rango
        for (int i = 0; i < nodo_actual.k; i++) {
            if (nodo_actual.llaves_valores[i].llave >= limite_inf && 
                nodo_actual.llaves_valores[i].llave <= limite_sup) {
                // expandir si falta espacio
                if (*cantidad >= *capacidad) {
                    *capacidad *= 2;
                    *resultados = realloc(*resultados, sizeof(Pair) * (*capacidad));
                }
                (*resultados)[*cantidad] = nodo_actual.llaves_valores[i];
                (*cantidad)++;
            }
        }
    } else {
        // revisar los pares del nodo interno
        for (int i = 0; i < nodo_actual.k; i++) {
            if (nodo_actual.llaves_valores[i].llave >= limite_inf && 
                nodo_actual.llaves_valores[i].llave <= limite_sup) {
                // expandir si falta espacio
                if (*cantidad >= *capacidad) {
                    *capacidad *= 2;
                    *resultados = realloc(*resultados, sizeof(Pair) * (*capacidad));
                }
                (*resultados)[*cantidad] = nodo_actual.llaves_valores[i];
                (*cantidad)++;
            }
        }
        
        // buscar en todos los hijos que puedan contener elementos en el rango
        for (int i = 0; i <= nodo_actual.k; i++) {
            int debe_buscar = 0;
            
            if (i == 0) {
                // el primer hijo contiene llaves <= llaves_valores[0]
                debe_buscar = (limite_inf <= nodo_actual.llaves_valores[0].llave);
            } else if (i == nodo_actual.k) {
                // el ultimo hijo contiene llaves > llaves_valores[k-1]
                debe_buscar = (limite_sup > nodo_actual.llaves_valores[nodo_actual.k - 1].llave);
            } else {
                // hijo intermedio contiene llaves en (llaves_valores[i-1], llaves_valores[i])
                int llave_min_hijo = nodo_actual.llaves_valores[i - 1].llave;
                int llave_max_hijo = nodo_actual.llaves_valores[i].llave;
                debe_buscar = (limite_inf <= llave_max_hijo) && (limite_sup > llave_min_hijo);
            }
            
            if (debe_buscar && nodo_actual.hijos[i] != -1) {
                buscar_rango_arbolB_recursivo(archivo, nodo_actual.hijos[i], limite_inf, limite_sup, tamano_nodo, resultados, cantidad, capacidad, stats);
            }
        }
    }
}

// funcion principal para buscar en arbol B
Pair* buscar_rango_arbolB(const char *nombre_archivo, int limite_inf, int limite_sup, int* cantidad_resultados, StatsIO* stats) {
    int capacidad = 1000;
    Pair* resultados = malloc(sizeof(Pair) * capacidad);
    int cantidad = 0;
    FILE *archivo = fopen(nombre_archivo, "rb");
    if (archivo == NULL) {
        printf("Error al abrir el archivo: %s\n", nombre_archivo);
        *cantidad_resultados = 0;
        return resultados;
    }
    int tamano_nodo = sizeof(Nodo);
    buscar_rango_arbolB_recursivo(archivo, 0, limite_inf, limite_sup, tamano_nodo, &resultados, &cantidad, &capacidad, stats);
    fclose(archivo);
    *cantidad_resultados = cantidad;
    return resultados;
}

// funcion auxiliar para encontrar la primera hoja en arbol B+
int encontrar_primera_hoja_BPlus(FILE *archivo, int indice_actual, int limite_inf, int tamano_nodo, StatsIO* stats) {
    Nodo nodo_actual;
    fseek(archivo, indice_actual * tamano_nodo, SEEK_SET);
    fread(&nodo_actual, tamano_nodo, 1, archivo);
    contar_lectura_busqueda(stats);
    
    // si es hoja se retorna su indice
    if (nodo_actual.es_interno == 0) {
        return indice_actual;
    }
    // si es nodo interno hay que encontrar el hijo apropiado
    int i = 0;
    while (i < nodo_actual.k && limite_inf > nodo_actual.llaves_valores[i].llave) {
        i++;
    }
    // recursivamente bajar al hijo apropiado
    if (nodo_actual.hijos[i] != -1) {
        return encontrar_primera_hoja_BPlus(archivo, nodo_actual.hijos[i], limite_inf, tamano_nodo, stats);
    }
    return -1;
}

// funcion auxiliar para recorrer las hojas del arbol B+ y recolectar resultados
void recorrer_hojas_BPlus(FILE *archivo, int indice_hoja_inicial, int limite_inf, int limite_sup, int tamano_nodo, Pair** resultados, int* cantidad, int* capacidad, StatsIO* stats) {
    int indice_hoja_actual = indice_hoja_inicial;
    
    // recorrer las hojas usando el puntero siguiente
    while (indice_hoja_actual != -1) {
        Nodo hoja_actual;
        fseek(archivo, indice_hoja_actual * tamano_nodo, SEEK_SET);
        fread(&hoja_actual, tamano_nodo, 1, archivo);
        contar_lectura_busqueda(stats);
        
        // revisar todos los pares de la hoja
        for (int i = 0; i < hoja_actual.k; i++) {
            int llave = hoja_actual.llaves_valores[i].llave;
            
            if (llave > limite_sup) { // si la llave es mayor al limite superior se detiene
                return;
            }    
            // esta en el rango y se agrega
            if (llave >= limite_inf && llave <= limite_sup) {
                if (*cantidad >= *capacidad) {
                    *capacidad *= 2;
                    *resultados = realloc(*resultados, sizeof(Pair) * (*capacidad));
                };
                (*resultados)[*cantidad] = hoja_actual.llaves_valores[i];
                (*cantidad)++;
            }
        }
        
        // si no hay siguiente hoja se termina
        if (hoja_actual.sgte == -1) {
            return;
        }
        // avanzar a la siguiente hoja
        indice_hoja_actual = hoja_actual.sgte;
    };
}

// funcion principal para buscar en arbol B+
Pair* buscar_rango_arbolBPlus(const char *nombre_archivo, int limite_inf, int limite_sup, int* cantidad_resultados, StatsIO* stats) {
    int capacidad = 1000;
    Pair* resultados = malloc(sizeof(Pair) * capacidad);
    int cantidad = 0;
    
    FILE *archivo = fopen(nombre_archivo, "rb");
    if (archivo == NULL) {
        printf("Error al abrir el archivo: %s\n", nombre_archivo);
        *cantidad_resultados = 0;
        return resultados;
    }
    
    int tamano_nodo = sizeof(Nodo);
    int indice_primera_hoja = encontrar_primera_hoja_BPlus(archivo, 0, limite_inf, tamano_nodo, stats);
    if (indice_primera_hoja != -1) {
        recorrer_hojas_BPlus(archivo, indice_primera_hoja, limite_inf, limite_sup, tamano_nodo, &resultados, &cantidad, &capacidad, stats);
    }
    fclose(archivo);
    *cantidad_resultados = cantidad;
    return resultados;
}

// funcion para buscar en un arbol B desde archivo
Pair* buscar_en_arbolB(const char *nombre_archivo, int limite_inf, int limite_sup, int* cantidad_resultados, StatsIO* stats) {
    return buscar_rango_arbolB(nombre_archivo, limite_inf, limite_sup, cantidad_resultados, stats);
}

// funcion para buscar en un arbol B+ desde archivo
Pair* buscar_en_arbolBPlus(const char *nombre_archivo, int limite_inf, int limite_sup, int* cantidad_resultados, StatsIO* stats) {
    return buscar_rango_arbolBPlus(nombre_archivo, limite_inf, limite_sup, cantidad_resultados, stats);
}