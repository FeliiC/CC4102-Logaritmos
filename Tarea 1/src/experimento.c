#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <nodo.h>
//#define M 10000     // tamaño inicial del arreglo de nodos

void ejecutar_experimento(int N){
    // Crear árboles
    int tamanoB, tamanoBPlus;
    Nodo **arbolB = inicializar_arbol(tamanoB);
    Nodo **arbolBPlus = inicializar_arbol(tamanoBPlus);

    // Insertar
    printf("\n-------- INSERTANDO EN ÁRBOL --------\n");
    clock_t inicio_creacion = clock();
    FILE* archivo = fopen("datos.bin", "rb");
    if (archivo == NULL) {
        printf("Error al abrir el archivo: datos.bin\n");
        liberar_arbol(arbolB, tamanoB);
        liberar_arbol(arbolBPlus, tamanoBPlus);
        return;
    }
    int pares_insertados=0;
    Pair pair;
    while(pares_insertados < N && fread(&pair, sizeof(Pair), 1, archivo) == 1){
        insertar_en_arbolB(arbolB, tamanoB, arbolB[0], pair.llave, pair.valor);
        insertar_en_arbolBPlus(arbolBPlus, tamanoBPlus, arbolBPlus[0], pair.llave, pair.valor);
        pares_insertados++;
    }
    fclose(archivo);
    clock_t fin_creacion = clock();
    double tiempo_creacion = ((double)(fin_creacion - inicio_creacion)) / CLOCKS_PER_SEC;

    // Escribir a disco
    printf("\n-------- ESCRIBIENDO EN DISCO --------\n");
    escribir_a_disco(arbolB, tamanoB, "arbolB.bin");
    escribir_a_disco(arbolBPlus, tamanoBPlus, "arbolBPlus.bin");

    // Búsquedas
    printf("\n-------- REALIZANDO BÚSQUEDAS --------\n");
    int llave_min = 1546300800;
    int llave_max = 1754006400;
    double tiempo_busqueda_B;
    double tiempo_busqueda_BPlus;
    srand(time(NULL));          // semilla para números aleatorios
    for (int i = 0; i<50; i++) {
        int l = llave_min + rand() % (llave_max - llave_min + 1);   // ver si esta en el rango esperado
        int u = l + 604.800;

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
    printf("\n-------- RESULTADOS EXPERIMENTO N = %d --------\n", N);
    printf("Tiempo de creación árbolB y árbolB+ : %.6f segundos", tiempo_creacion);

    printf("\n-------- ÁRBOL B --------\n");
    printf("I/Os durante inserción: \n");
    printf("Tamaño árbol: %d nodos\n", tamanoB);
    printf("Tiempo promedio de búsqueda: %.6f segundos\n", promedio_busqueda_B);
    printf("Promedio I/Os de búsqueda: \n");

    printf("\n-------- ÁRBOL B+ --------\n");
    printf("I/Os durante inserción: \n");
    printf("Tamaño árbol: %d nodos\n", tamanoBPlus);
    printf("Tiempo promedio de búsqueda: %.6f segundos\n", promedio_busqueda_BPlus);
    printf("Promedio I/Os de búsqueda: \n");

    liberar_arbol(arbolB, tamanoB);
    liberar_arbol(arbolBPlus, tamanoBPlus);
}

void main(){
    printf("\n-------- INICIO EXPERIMENTOS --------\n");
    for(int i=15; i<27; i++){
        int N = 2^i;
        printf("\n-------- EJECUTANDO EXPERIMENTO N = %d --------\n", N);
        ejecutar_experimento(N);
    }
    printf("\n-------- FIN EXPERIMENTOS --------\n");
}

