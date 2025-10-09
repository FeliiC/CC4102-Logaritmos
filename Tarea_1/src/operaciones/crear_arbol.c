#include <stdio.h>
#include <stdlib.h>


/* Inicializar nodo
    @param es_interno: Indica si es un nodo interno o una hoja

    @return Nodo inicializado con valores 0
*/
Nodo* inicializar_nodo(int es_interno){
    Nodo* n = malloc(sizeof(Nodo));     
    n->es_interno = es_interno; 
    n->k = 0;
    n->sgte = -1;
    for (int i = 0; i < B; i++) {
        n->llaves_valores[i].llave = 0;
        n->llaves_valores[i].valor = 0.0f;
    }
    for (int i = 0; i < B+1; i++) {
        n->hijos[i] = -1;
    }
    return n;
}

/* Inicializar arbol
    @param tamano_arbol: Tamaño del arbol para pedir memoria

    @return Arreglo de nodos con nodo raiz inicializado
*/ 
Nodo** inicializar_arbol(int tamano_arbol){
    Nodo **arbol = malloc(sizeof(Nodo*) * 500000);
    for (int i = 0; i < 1000; i++) {
        arbol[i] = NULL;
    }
    // Crear nodo raiz
    arbol[0] = inicializar_nodo(0);   
    return arbol;
}

/* Libera la memoria de un arbol
    @param arbol: Arreglo de nodos a liberar
    @param tamano_arbol: Tamaño del arbol para pedir memoria
*/
void liberar_arbol(Nodo **arbol, int tamano_arbol) {
    for (int i = 0; i < tamano_arbol; i++) {
        free(arbol[i]);
    }
    free(arbol);
}


/* Escribir un arbol a disco
    @param arbol: Arreglo de nodos a escribir
    @param tamano_arbol: Tamaño del arbol para pedir memoria
    @param nombre_archivo: Archivo donde escribir arbol
*/
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
