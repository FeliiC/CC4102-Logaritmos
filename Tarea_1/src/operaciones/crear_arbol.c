#include <stdio.h>
#include <stdlib.h>

/*
// Inicializar nodo
Nodo* inicializar_nodo(int es_interno){
    Nodo* n;
    n->es_interno= es_interno; 
    n->k = 0;
    n->sgte = -1;
    return n;
}

// Función para inicializar un árbol
Nodo** inicializar_arbol(int tamano_arbol){
    printf("Creando arbol...");
    Nodo **arbol = malloc(sizeof(Nodo*)*tamano_arbol);   // iniciamos con espacio para 10000 nodos 
    printf("Se pidio memoria");
    arbol[0]=inicializar_nodo(0);
    printf("Arbol creado...");
    return arbol;
}
*/

// Inicializar nodo 
Nodo* inicializar_nodo(int es_interno){
    Nodo* n = malloc(sizeof(Nodo));     
    n->es_interno = es_interno; 
    n->k = 0;
    n->sgte = -1;
    // Inicializar arrays para evitar valores basura
    for (int i = 0; i < B; i++) {
        n->llaves_valores[i].llave = 0;
        n->llaves_valores[i].valor = 0.0f;
    }
    for (int i = 0; i < B+1; i++) {
        n->hijos[i] = -1;
    }
    return n;
}

// Función para inicializar un árbol - CORREGIDA
Nodo** inicializar_arbol(int tamano_arbol){
    Nodo **arbol = malloc(sizeof(Nodo*) * tamano_arbol);
    // Inicializar todos los punteros a NULL
    for (int i = 0; i < tamano_arbol; i++) {
        arbol[i] = NULL;
    }
    // Crear nodo raíz
    arbol[0] = inicializar_nodo(0);   
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

// Función para contar la cantidad de nodos en un archivo de árbol
int contar_nodos_en_arbol(const char* nombre_archivo) {
    FILE* archivo = fopen(nombre_archivo, "rb");
    if (archivo == NULL) {
        printf("Error: No se pudo abrir el archivo %s\n", nombre_archivo);
        return -1;
    }
    
    // Calcular el tamaño del archivo y número de nodos
    fseek(archivo, 0, SEEK_END);
    long tamano_archivo = ftell(archivo);
    rewind(archivo);
    
    int tamano_nodo = sizeof(Nodo);
    int num_nodos = tamano_archivo / tamano_nodo;
    
    if (tamano_archivo % tamano_nodo != 0) {
        printf("Advertencia: El tamaño del archivo %s no es múltiplo del tamaño del nodo\n", nombre_archivo);
    }
    
    fclose(archivo);
    
    return num_nodos;
}