#include <stdio.h>

/* Inicializar contadores
    @param stats_global: Estructura con contadores de IOs
*/
void iniciar_estadisticas(StatsIO* stats_global) {
    stats_global->lecturas_busqueda = 0;
    stats_global->escrituras_insercion = 0;
    stats_global->lecturas_insercion = 0;
    stats_global->total_operaciones = 0;
}

/* Aumentar IOs de lectura en busqueda
    @param stats_global: Estructura con contadores de IOs
*/
void contar_lectura_busqueda(StatsIO* stats_global) {
    stats_global->lecturas_busqueda++;
    stats_global->total_operaciones++;
}

/* Aumentar IOs de lectura en insercion
    @param stats_global: Estructura con contadores de IOs
*/
void contar_lectura_insercion(StatsIO* stats_global) {
    stats_global->lecturas_insercion++;
    stats_global->total_operaciones++;
}

/* Aumentar IOs de escritura en insercion
    @param stats_global: Estructura con contadores de IOs
*/
void contar_escritura_insercion(StatsIO* stats_global) {
    stats_global->escrituras_insercion++;
    stats_global->total_operaciones++;
}

/* Imprimir contadores
    @param stats_global: Estructura con contadores de IOs
*/
void imprimir_estadisticas(StatsIO* stats_global) {
    printf("=== ESTADÍSTICAS I/O ===\n");
    printf("Lecturas en búsqueda: %d\n", stats_global->lecturas_busqueda);
    printf("Lecturas en inserción: %d\n", stats_global->lecturas_insercion);
    printf("Escrituras en inserción: %d\n", stats_global->escrituras_insercion);
    printf("Total operaciones I/O: %d\n", stats_global->total_operaciones);
}
