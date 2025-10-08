#include <stdio.h>

/* Leer arbol desde un archivo binario
    @param arbol: Arbol a leer
*/
void leer_pares_desde_archivo(const char* nombre_archivo) {
    FILE* archivo = fopen(nombre_archivo, "rb");
    if (!archivo) {
        printf("Error: No se pudo abrir el archivo %s\n", nombre_archivo);
        return;
    }
    
    Pair pares[10];
    int leidos = fread(pares, sizeof(Pair), 10, archivo);
    printf("Primeros %d pares del archivo '%s':\n", leidos, nombre_archivo);
    printf("----------------------------------------\n");
    for (int i = 0; i < leidos; i++) {
        printf("%d\t%.4f\n", pares[i].llave, pares[i].valor);
    }
    fclose(archivo);
}

/* Escribir pares a un archivo de texto
    @param pares: Arreglo de pares a escribir
    @param cantidad: Cantidad de pares
    @param nombre_archivo: Archivo de texto donde escribir
*/
void escribir_pares_a_txt(Pair* pares, int cantidad, const char* nombre_archivo) {
    FILE* archivo = fopen(nombre_archivo, "w");
    if (archivo == NULL) {
        printf("Error al crear el archivo: %s\n", nombre_archivo);
        return;
    }
    // escribir pares
    for (int i = 0; i < cantidad; i++) {
        fprintf(archivo, "%d\t%.4f\n", pares[i].llave, pares[i].valor);
    }
    fclose(archivo);
}

// Función auxiliar para agregar resultados a un archivo de texto (modo append)
/* Guardar resultados en archivo de texto
    @param i: Exponente de cantidad de pares N=2^i
    @param tiempo_creacion: Tiempo de creacion del arbol
    @param ios_insercion: IOs al insertar
    @param tamano_arbol: Tamaño del arbol resultante
    @param tiempo_busqueda: Tiempo promedio de busqueda
    @param ios_busqueda: IOs al buscar
    @param porc_encontrado: Porcentaje de pares encontrados
    @param nombre_archivo: Archivo donde escribir resultados
*/
/*
void agregar_resultados_a_txt(int i, double tiempo_creacion, int ios_insercion, 
                             int tamano_arbol, double tiempo_busqueda, 
                             int ios_busqueda, double porc_encontrado, 
                             const char* nombre_archivo) {
    FILE* archivo = fopen(nombre_archivo, "a");  
    if (archivo == NULL) {
        printf("Error al abrir el archivo: %s\n", nombre_archivo);
        return;
    }  
    // Escribir encabezado si el archivo esta vacío
    fseek(archivo, 0, SEEK_END);
    if (ftell(archivo) == 0) {
        fprintf(archivo, "N\tTiempo_Creacion(s)\tIOs_Insercion\tTamano_Arbol\tTiempo_Busqueda(s)\tIOs_Busqueda\tPorcentaje_Encontrado(%%)\n");
    }
    // Escribir resultados
    fprintf(archivo, "2^%d\t%.6f\t%d\t%d\t%.6f\t%d\t%.2f\n", 
            i, tiempo_creacion, ios_insercion, tamano_arbol, 
            tiempo_busqueda, ios_busqueda, porc_encontrado);
    fclose(archivo);
}

*/

void agregar_resultados_a_txt(int i, double tiempo_creacion, int ios_insercion, 
                             int tamano_arbol, double tiempo_busqueda, 
                             int ios_busqueda, double porc_encontrado, 
                             const char* nombre_archivo) {
    FILE* archivo = fopen(nombre_archivo, "a");
    if (archivo == NULL) {
        printf("Error al abrir el archivo: %s\n", nombre_archivo);
        return;
    }
    // Escribir encabezado si el archivo esta vacío
    fseek(archivo, 0, SEEK_END);
    if (ftell(archivo) == 0) {
        fprintf(archivo, "%-8s %-16s %-14s %-12s %-18s %-12s %-10s\n", 
                "N", "Tiempo_Creacion", "IOs_Insercion", "Tamano_Arbol", 
                "Tiempo_Busqueda", "IOs_Busqueda", "Porcentaje_Encontrado");
        fprintf(archivo, "%-8s %-16s %-14s %-12s %-18s %-12s %-10s\n", 
                "---", "---------------", "-------------", "------------", 
                "----------------", "------------", "-----------");
    }
    char n_str[10];
    snprintf(n_str, sizeof(n_str), "2^%d", i);
    // Escribir los resultados con formato alineado
    fprintf(archivo, "%-8s %-16.6f %-14d %-12d %-18.6f %-12d %-10.2f\n", 
            n_str, tiempo_creacion, ios_insercion, tamano_arbol, 
            tiempo_busqueda, ios_busqueda, porc_encontrado);
    
    fclose(archivo);
}