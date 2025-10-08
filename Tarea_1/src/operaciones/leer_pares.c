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

void main(){
    leer_pares_desde_archivo("../../datos_bin/arbolBPlus.bin");
}