#ifndef TRNS_H
#define TRNS_H

#include <string.h>
#include <stdlib.h>
#include "afnd.h"

#define MAX_NAME 256

/**
 * Estructura intermedia de estado
 * En "componentes" se marcan con 1 los indices de los estados del afnd
 * contenidos en este estado del afd
 */
struct Estado {
	char *nombre;
	int tipo;
	int *componentes;
};

/**
 * Estructura intermedia de transicion
 */
struct Transicion {
	char *origen;
	char *destino;
	char *simbolo;
};

AFND * AFNDTransforma(AFND * afnd);
int existe_en_afd(char *nombre, struct Estado *estados, int len);
char *nombreParaEstado(AFND *afnd, int *componentes, int num_estados_afnd);
void crearEstado(struct Estado *estado, char *nombre, int *array_destinos, int es_final, int num_estados_afd, int num_estados_afnd);
void crearTransicion(struct Transicion *tr, char *ori, char *dst, char *sim);

#endif
