#include "estadisticas.h"

// Definir la variable global
EstadisticasIO stats_global = {0, 0, 0, 0};

void iniciar_estadisticas() {
    stats_global.lecturas_busqueda = 0;
    stats_global.escrituras_insercion = 0;
    stats_global.lecturas_insercion = 0;
    stats_global.total_operaciones = 0;
}

void contar_lectura_busqueda() {
    stats_global.lecturas_busqueda++;
    stats_global.total_operaciones++;
}

void contar_lectura_insercion() {
    stats_global.lecturas_insercion++;
    stats_global.total_operaciones++;
}

void contar_escritura_insercion() {
    stats_global.escrituras_insercion++;
    stats_global.total_operaciones++;
}

void imprimir_estadisticas() {
    printf("=== ESTADÍSTICAS I/O ===\n");
    printf("Lecturas en búsqueda: %d\n", stats_global.lecturas_busqueda);
    printf("Lecturas en inserción: %d\n", stats_global.lecturas_insercion);
    printf("Escrituras en inserción: %d\n", stats_global.escrituras_insercion);
    printf("Total operaciones I/O: %d\n", stats_global.total_operaciones);
}

EstadisticasIO obtener_estadisticas() {
    return stats_global;
}