#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "estructuras/nodo.c"
#include "operaciones/buscar.c"
#include "operaciones/crear_arbol.c"
#include "operaciones/insertar.c"
#include "operaciones/stats.c"

/* Experimento de insercion y busqueda en un arbol 
    @param N: Cantidad de pares para el experimento
*/
void ejecutar_experimento(int N){
    StatsIO stats_arbolB;
    StatsIO stats_arbolBPlus;
    iniciar_estadisticas(&stats_arbolB);
    iniciar_estadisticas(&stats_arbolBPlus);
    printf("Crear arbol\n");
    // Crear árboles
    int tamanoB = 1;
    int tamanoBPlus = 1;
    int raizB = 0, raizBPlus = 0;
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
        insertar_en_arbolB(&arbolB, &tamanoB, &raizB, pair.llave, pair.valor, &stats_arbolB);
        clock_t fin_creacion_B = clock();
        tiempo_creacion_B += ((double)(fin_creacion_B - inicio_creacion_B)) / CLOCKS_PER_SEC;
        // Crear arbolB+
        clock_t inicio_creacion_BPlus = clock();
        insertar_en_arbolBPlus(&arbolBPlus, &tamanoBPlus, &raizBPlus, pair.llave, pair.valor, &stats_arbolBPlus);
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
        int l = llave_min + rand() % (llave_max - llave_min + 1);   
        int u = l + 604800;

        // Árbol B
        int cantidad_B=0;
        clock_t inicio_busqueda_B = clock();
        Pair* resultados_B = buscar_en_arbolB("../datos_bin/arbolB.bin", l, u, &cantidad_B, &stats_arbolB);
        clock_t fin_busqueda_B = clock();
        tiempo_busqueda_B += ((double)(fin_busqueda_B - inicio_busqueda_B)) / CLOCKS_PER_SEC;
        total_resultados_B += cantidad_B;
        //Árbol B+
        int cantidad_BPlus=0;
        clock_t inicio_busqueda_BPlus = clock();
        Pair* resultados_BPlus = buscar_en_arbolBPlus("../datos_bin/arbolBPlus.bin", l, u, &cantidad_BPlus, &stats_arbolBPlus);
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
    printf("I/Os durante insercion: %d\n", stats_arbolB.escrituras_insercion + stats_arbolB.lecturas_insercion);
    printf("Tamano arbol: %d nodos\n", tamanoB);
    printf("Tiempo promedio de busqueda: %.6f segundos\n", promedio_busqueda_B);
    printf("Promedio I/Os de busqueda: %d\n", stats_arbolB.lecturas_busqueda);
    printf("Porcentaje promedio de datos encontrados: %.2f%%\n", porcentaje_promedio_B);

    printf("--- Arbol B+ ---\n");
    printf("Tiempo de creacion: %.6f segundos\n", tiempo_creacion_BPlus);
    printf("I/Os durante insercion: %d\n", stats_arbolBPlus.escrituras_insercion + stats_arbolBPlus.lecturas_insercion);
    printf("Tamano arbol: %d nodos\n", tamanoBPlus);
    printf("Tiempo promedio de busqueda: %.6f segundos\n", promedio_busqueda_BPlus);
    printf("Promedio I/Os de busqueda: %d\n", stats_arbolBPlus.lecturas_busqueda);
    printf("Porcentaje promedio de datos encontrados: %.2f%%\n", porcentaje_promedio_BPlus);

    liberar_arbol(arbolB, tamanoB);
    liberar_arbol(arbolBPlus, tamanoBPlus);
}

// Experimento de resultados de una busqueda por rango
void ejecutar_experimento2(){
    int N1 = pow(2, 15);
    int N2 = pow(2, 20);
    // Inicializar arboles B
    StatsIO stats_arbolB1;
    StatsIO stats_arbolB2;
    iniciar_estadisticas(&stats_arbolB1);
    iniciar_estadisticas(&stats_arbolB2);
    int tamanoB1 = 1, tamanoB2 = 1;
    int raizB1 = 0, raizB2 = 0;
    Nodo **arbolB1 = inicializar_arbol(tamanoB1);
    Nodo **arbolB2 = inicializar_arbol(tamanoB2);
    printf("Insertando en arbol\n");
    // Insertar con N1
    FILE* archivo = fopen("../datos_bin/datos.bin", "rb");
    if (archivo == NULL) {
        printf("Error al abrir el archivo: datos.bin\n");
        liberar_arbol(arbolB1, tamanoB1);
        liberar_arbol(arbolB2, tamanoB2);
        return;
    }
    int pares_insertados1=0;
    Pair pair1;
    while(pares_insertados1 < N1 && fread(&pair1, sizeof(Pair), 1, archivo) == 1){
        insertar_en_arbolB(&arbolB1, &tamanoB1, &raizB1, pair1.llave, pair1.valor, &stats_arbolB1);
        pares_insertados1++;
    }
    fclose(archivo);
    // Insertar con N2
    FILE* archivo = fopen("../datos_bin/datos.bin", "rb");
    int pares_insertados2=0;
    Pair pair2;
    while(pares_insertados2 < N2 && fread(&pair2, sizeof(Pair), 1, archivo) == 1){
        insertar_en_arbolB(&arbolB2, &tamanoB2, &raizB2, pair2.llave, pair2.valor, &stats_arbolB2);
        pares_insertados2++;
    }
    fclose(archivo);
    // Escribir a disco
    printf("Escribiendo en disco\n");
    printf("-> escribir arbolB con N=2^15\n");
    escribir_a_disco(arbolB1, tamanoB1, "../datos_bin/arbolB_N15.bin");
    printf("-> escribir arbolB con N=2^20\n");
    escribir_a_disco(arbolB2, tamanoB2, "../datos_bin/arbolB_N20.bin");
    // Realizando busqueda
    printf("Realizando busqueda\n");
    int llave_min = 1546300800;
    int llave_max = 1754006400;
    srand(time(NULL));
    int l = llave_min + rand() % (llave_max - llave_min + 1);  
    int u = l + 604800;
    // Busqueda en N1
    int cantidad_B1=0;
    Pair* resultados_B1 = buscar_en_arbolB("../datos_bin/arbolB_N15.bin", l, u, &cantidad_B1, &stats_arbolB1);
    // Busqueda en N2
    int cantidad_B2=0;
    Pair* resultados_B2 = buscar_en_arbolB("../datos_bin/arbolB_N20.bin", l, u, &cantidad_B2, &stats_arbolB2);
    // Imprimir resultados
    printf("\n-------- Resultados para N=2^15 --------\n");
    for (int i = 0; i < cantidad_B1; i++) {
        printf("%d\t%.4f\n", resultados_B1[i].llave, resultados_B1[i].valor);
    }
    printf("\n-------- Resultados para N=2^20 --------\n");
    for (int i = 0; i < cantidad_B2; i++) {
        printf("%d\t%.4f\n", resultados_B2[i].llave, resultados_B2[i].valor);
    }
    liberar_arbol(arbolB1, tamanoB1);
    liberar_arbol(arbolB2, tamanoB2);
}

// Funcion principal para ejecutar experimentos
void main(){
    printf("\n-------- INICIO EXPERIMENTOS --------\n");
    for(int i=15; i<20; i++){       
        int N = pow(2, i);
        printf("\n-------- EJECUTANDO EXPERIMENTO N = 2^%d --------\n", i);
        ejecutar_experimento(N);
    }
    printf("\n-------- EJECUTANDO EXPERIMENTO BUSQUEDA PARA N=2^15 Y N=2^20 --------\n");
    ejecutar_experimento2();
    printf("\n-------- FIN EXPERIMENTOS --------\n");
}

