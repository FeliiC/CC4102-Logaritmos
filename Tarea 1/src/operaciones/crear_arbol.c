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

// Función para inicializar un árbol
Nodo* inicializar_arbol(int tamano_arbol){
    Nodo *arbol = malloc(sizeof(Nodo)*tamano_arbol);   // iniciamos con espacio para 10000 nodos 
    arbol[0]=inicializar_nodo(0);
    return arbol;
}

// Función para liberar memoria del árbol
void liberar_arbol(Nodo **arbol, int tamano_arbol) {
    for (int i = 0; i < tamano_arbol; i++) {
        free(arbol[i]);
    }
    free(arbol);
}


void escribir_a_disco(Nodo **arbol, int tamano_arbol, const char* nombre_archivo){
    FILE* archivo = fopen(nombre_archivo, "wb");
    if (archivo == NULL) {
        printf("Error al abrir el archivo: %s\n", nombre_archivo);
        return;
    }

    for (int i = 0; i < tamano_arbol; i++) {
        fwrite(arbol[i], sizeof(Nodo), 1, archivo);
    }
    fclose(archivo);
}