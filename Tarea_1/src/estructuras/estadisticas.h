#ifndef ESTADISTICAS_H
#define ESTADISTICAS_H

#include <stdio.h>

typedef struct {
    int lecturas_busqueda;
    int escrituras_insercion;
    int lecturas_insercion;
    int total_operaciones;
} EstadisticasIO;

// Variable global
extern EstadisticasIO stats_global;

// Funciones
void iniciar_estadisticas();
void contar_lectura_busqueda();
void contar_lectura_insercion();
void contar_escritura_insercion();
void imprimir_estadisticas();
EstadisticasIO obtener_estadisticas();

#endif