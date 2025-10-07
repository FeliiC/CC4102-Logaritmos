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
    int tamanoB = 1;
    int tamanoBPlus = 1;
    int raizB, raizBPlus = 0;
    Nodo **arbolB = inicializar_arbol(tamanoB);
    Nodo **arbolBPlus = inicializar_arbol(tamanoBPlus);

    // Insertar
    printf("Insertando en arbol\n");
    FILE* archivo = fopen("../datos_bin/datos.bin", "rb");
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
    escribir_a_disco(arbolB, tamanoB, "../datos_bin/arbolB.bin");
    printf("-> escribir arbolB+\n");
    escribir_a_disco(arbolBPlus, tamanoBPlus, "../datos_bin/arbolBPlus.bin");

    // Búsquedas
    printf("Realizando busquedas\n");
    int llave_min = 1546300800;
    int llave_max = 1754006400;
    double tiempo_busqueda_B = 0.0;
    double tiempo_busqueda_BPlus = 0.0;
    int total_resultados_B=0;
    int total_resultados_BPlus=0;
    srand(time(NULL));          // semilla para números aleatorios
    for (int i = 0; i<50; i++) {
        int l = llave_min + rand() % (llave_max - llave_min + 1);   // ver si esta en el rango esperado
        int u = l + 604800;

        // Árbol B
        int cantidad_B=0;
        clock_t inicio_busqueda_B = clock();
        Pair* resultados_B = buscar_rango_desde_archivo("../datos_bin/arbolB.bin", l, u, &cantidad_B);
        clock_t fin_busqueda_B = clock();
        tiempo_busqueda_B += ((double)(fin_busqueda_B - inicio_busqueda_B)) / CLOCKS_PER_SEC;
        total_resultados_B += cantidad_B;
        //Árbol B+
        int cantidad_BPlus=0;
        clock_t inicio_busqueda_BPlus = clock();
        Pair* resultados_BPlus = buscar_rango_desde_archivo("../datos_bin/arbolBPlus.bin", l, u, &cantidad_BPlus);
        clock_t fin_busqueda_BPlus = clock();
        tiempo_busqueda_BPlus += ((double)(fin_busqueda_BPlus - inicio_busqueda_BPlus)) / CLOCKS_PER_SEC;
        total_resultados_BPlus += cantidad_BPlus;
    }
    double promedio_busqueda_B = tiempo_busqueda_B / 50;
    double promedio_busqueda_BPlus = tiempo_busqueda_BPlus / 50;
    // Calcular porcentajes
    double porcentaje_promedio_B = ((double)total_resultados_B / (50.0 * N)) * 100.0;
    double porcentaje_promedio_BPlus = ((double)total_resultados_BPlus / (50.0 * N)) * 100.0;

    // Resultados
    printf("\n-------- Resultados experimento --------\n", N);

    printf("--- Arbol B ---\n");
    printf("Tiempo de creacion: %.6f segundos\n", tiempo_creacion_B);
    printf("I/Os durante insercion: \n");
    printf("Tamano arbol: %d nodos\n", tamanoB);
    printf("Tiempo promedio de busqueda: %.6f segundos\n", promedio_busqueda_B);
    printf("Promedio I/Os de busqueda: \n");
    printf("Porcentaje promedio de datos encontrados: %.2f%%\n", porcentaje_promedio_B);

    printf("--- Arbol B+ ---\n");
    printf("Tiempo de creacion: %.6f segundos\n", tiempo_creacion_BPlus);
    printf("I/Os durante insercion: \n");
    printf("Tamano arbol: %d nodos\n", tamanoBPlus);
    printf("Tiempo promedio de busqueda: %.6f segundos\n", promedio_busqueda_BPlus);
    printf("Promedio I/Os de busqueda: \n");
    printf("Porcentaje promedio de datos encontrados: %.2f%%\n", porcentaje_promedio_BPlus);

    liberar_arbol(arbolB, tamanoB);
    liberar_arbol(arbolBPlus, tamanoBPlus);
}

void main(){
    printf("\n-------- INICIO EXPERIMENTOS --------\n");
    for(int i=15; i<16; i++){       
        int N = pow(2, i);
        printf("\n-------- EJECUTANDO EXPERIMENTO N = 2^%d --------\n", i);
        ejecutar_experimento(N);
    }
    printf("\n-------- FIN EXPERIMENTOS --------\n");
}

