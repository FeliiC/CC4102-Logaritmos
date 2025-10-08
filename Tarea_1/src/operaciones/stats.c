#include <stdio.h>

// Definir la variable global
void iniciar_estadisticas(StatsIO* stats_global) {
    stats_global->lecturas_busqueda = 0;
    stats_global->escrituras_insercion = 0;
    stats_global->lecturas_insercion = 0;
    stats_global->total_operaciones = 0;
}

void contar_lectura_busqueda(StatsIO* stats_global) {
    stats_global->lecturas_busqueda++;
    stats_global->total_operaciones++;
}

void contar_lectura_insercion(StatsIO* stats_global) {
    stats_global->lecturas_insercion++;
    stats_global->total_operaciones++;
}

void contar_escritura_insercion(StatsIO* stats_global) {
    stats_global->escrituras_insercion++;
    stats_global->total_operaciones++;
}

void imprimir_estadisticas(StatsIO* stats_global) {
    printf("=== ESTADÍSTICAS I/O ===\n");
    printf("Lecturas en búsqueda: %d\n", stats_global->lecturas_busqueda);
    printf("Lecturas en inserción: %d\n", stats_global->lecturas_insercion);
    printf("Escrituras en inserción: %d\n", stats_global->escrituras_insercion);
    printf("Total operaciones I/O: %d\n", stats_global->total_operaciones);
}
