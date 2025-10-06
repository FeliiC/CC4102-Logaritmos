#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "estructuras/nodo.c"
#include "operaciones/buscar.c"
#include "operaciones/crear_arbol.c"
#include "operaciones/insertar.c"
#define M 1000     // tamano inicial del arreglo de nodos

void ejecutar_experimento(int N){
    printf("Crear arbol\n");
    // Crear árboles
    int tamanoB, tamanoBPlus;
    int raizB, raizBPlus = 0;
    Nodo **arbolB = inicializar_arbol(tamanoB);
    Nodo **arbolBPlus = inicializar_arbol(tamanoBPlus);

    // Insertar
    printf("Insertando en arbol\n");
    FILE* archivo = fopen("datos.bin", "rb");
    if (archivo == NULL) {
        printf("Error al abrir el archivo: datos.bin\n");
        liberar_arbol(arbolB, tamanoB);
        liberar_arbol(arbolBPlus, tamanoBPlus);
        return;
    }
    int pares_insertados=0;
    Pair pair;
    double tiempo_creacion_B = 0.0;
    double tiempo_creacion_BPlus = 0.0;
    while(pares_insertados < N && fread(&pair, sizeof(Pair), 1, archivo) == 1){
        // Crear arbolB
        clock_t inicio_creacion_B = clock();
        insertar_en_arbolB(&arbolB, &tamanoB, &raizB, pair.llave, pair.valor);
        clock_t fin_creacion_B = clock();
        tiempo_creacion_B += ((double)(fin_creacion_B - inicio_creacion_B)) / CLOCKS_PER_SEC;
        // Crear arbolB+
        clock_t inicio_creacion_BPlus = clock();
        insertar_en_arbolBPlus(&arbolBPlus, &tamanoBPlus, &raizBPlus, pair.llave, pair.valor);
        clock_t fin_creacion_BPlus = clock();
        tiempo_creacion_BPlus += ((double)(fin_creacion_BPlus - inicio_creacion_BPlus)) / CLOCKS_PER_SEC;

        pares_insertados++;
    }
    fclose(archivo);

    // Escribir a disco
    printf("Escribiendo en disco\n");
    printf("-> escribir arbolB\n");
    escribir_a_disco(arbolB, tamanoB, "arbolB.bin");
    printf("-> escribir arbolB+\n");
    escribir_a_disco(arbolBPlus, tamanoBPlus, "arbolBPlus.bin");

    // Búsquedas
    printf("Realizando busquedas\n");
    int llave_min = 1546300800;
    int llave_max = 1754006400;
    double tiempo_busqueda_B = 0.0;
    double tiempo_busqueda_BPlus = 0.0;
    srand(time(NULL));          // semilla para números aleatorios
    for (int i = 0; i<50; i++) {
        int l = llave_min + rand() % (llave_max - llave_min + 1);   // ver si esta en el rango esperado
        int u = l + 604800;

        // Árbol B
        clock_t inicio_busqueda_B = clock();
        buscar_rango_desde_archivo("arbolB.bin", l, u);
        clock_t fin_busqueda_B = clock();
        tiempo_busqueda_B += ((double)(fin_busqueda_B - inicio_busqueda_B)) / CLOCKS_PER_SEC;
        //Árbol B+
        clock_t inicio_busqueda_BPlus = clock();
        buscar_rango_desde_archivo("arbolBPlus.bin", l, u);
        clock_t fin_busqueda_BPlus = clock();
        tiempo_busqueda_BPlus += ((double)(fin_busqueda_BPlus - inicio_busqueda_BPlus)) / CLOCKS_PER_SEC;
    }
    double promedio_busqueda_B = tiempo_busqueda_B / 50;
    double promedio_busqueda_BPlus = tiempo_busqueda_BPlus / 50;

    // Resultados
    printf("\n-------- Resultados experimento --------\n", N);

    printf("--- Arbol B ---\n");
    printf("Tiempo de creacion arbolB: %.6f segundos\n", tiempo_creacion_B);
    printf("I/Os durante insercion: \n");
    printf("Tamano arbol: %d nodos\n", tamanoB);
    printf("Tiempo promedio de busqueda: %.6f segundos\n", promedio_busqueda_B);
    printf("Promedio I/Os de busqueda: \n");

    printf("--- Arbol B+ ---\n");
    printf("Tiempo de creacion arbolB: %.6f segundos\n", tiempo_creacion_BPlus);
    printf("I/Os durante insercion: \n");
    printf("Tamano arbol: %d nodos\n", tamanoBPlus);
    printf("Tiempo promedio de busqueda: %.6f segundos\n", promedio_busqueda_BPlus);
    printf("Promedio I/Os de busqueda: \n");

    liberar_arbol(arbolB, tamanoB);
    liberar_arbol(arbolBPlus, tamanoBPlus);
}

void main(){
    printf("\n-------- INICIO EXPERIMENTOS --------\n");
    for(int i=15; i<27; i++){       
        int N = pow(2, i);
        printf("\n-------- EJECUTANDO EXPERIMENTO N = 2^%d --------\n", i);
        ejecutar_experimento(N);
    }
    printf("\n-------- FIN EXPERIMENTOS --------\n");
}

