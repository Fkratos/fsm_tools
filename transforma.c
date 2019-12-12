#include "transforma.h"

/**
 * Devuelve si existe un estado (dado su nombre) en la columna 'estados' de la tabla
 */
int existe_en_afd(char *nombre, struct Estado *estados, int len) {

	int i;
	for (i = 0; i < len; i++)
		if (!strcmp(nombre, estados[i].nombre))
			return 1;
	return 0;
}


/**
 * Genera un nombre para un nuevo estado dados sus estados componentes
 */
char *nombreParaEstado(AFND *afnd, int *componentes, int num_estados_afnd) {

	char *nombre;
	int i;

	nombre = (char *) malloc(MAX_NAME * sizeof(char));
	strcpy(nombre, "");
	for(i = 0; i < num_estados_afnd; i++) {
		if(componentes[i] == 1) {
			if (!strstr(nombre, AFNDNombreEstadoEn(afnd, i)))
				strcat(nombre, AFNDNombreEstadoEn(afnd, i));
		}
	}
	return nombre;
}


/**
 * Inicializa un estado pasado por parametro dados su nombre y componentes
 */
void crearEstado(struct Estado *estado, char *nombre, int *array_destinos,
		int es_final, int num_estados_afd, int num_estados_afnd) {

	int i;
	(*estado).componentes = (int*) calloc(num_estados_afnd, sizeof(int));
	for (i = 0; i < num_estados_afnd; i++)
		(*estado).componentes[i] = array_destinos[i];

	(*estado).nombre = (char *) malloc((strlen(nombre) + 1) * sizeof(char));
	strcpy((*estado).nombre, nombre);

	if (es_final)
		(*estado).tipo = FINAL;
	else
		(*estado).tipo = NORMAL;
}


/**
 * Inicializa una transicion pasada por parametro dados su origen, destino y simbolo
 */
void crearTransicion(struct Transicion *tr, char *ori, char *dst, char *sim) {

	(*tr).origen = (char*) malloc((strlen(ori) + 1) * sizeof(char));
	strcpy((*tr).origen, ori);

	(*tr).destino = (char*) malloc((strlen(dst) + 1) * sizeof(char));
	strcpy((*tr).destino, dst);

	(*tr).simbolo = (char*) malloc((strlen(sim) + 1) * sizeof(char));
	strcpy((*tr).simbolo, sim);
}


/**
 * Libera las listas de estados y transiciones
 */
void liberaMemoria(struct Estado *estados_afd, struct Transicion *transiciones_afd,
		int *array_destinos, int num_estados_afd, int num_trans_afd) {

	int i;
	for (i = 0; i < num_estados_afd; i++) {
		free(estados_afd[i].nombre);
		free(estados_afd[i].componentes);
	}
	free(estados_afd);
	for (i = 0; i < num_trans_afd; i++) {
		free(transiciones_afd[i].origen);
		free(transiciones_afd[i].destino);
		free(transiciones_afd[i].simbolo);
	}
	free(transiciones_afd);
	free(array_destinos);
}


/**
 * Transforma un AFND en un AFD
 */
AFND * AFNDTransforma(AFND * afnd) {

	AFND *afd = NULL;
	struct Estado *estados_afd = NULL;
	struct Transicion *transiciones_afd = NULL;
	int num_estados_afd = 1, num_simbolos, num_trans_afd = 0, num_estados_afnd;
	int i, j, k, l, estado, es_final = 0, tipo, hay_destinos = 0, *array_destinos;
	char *nombre_temp = NULL;

	num_estados_afnd = AFNDNumEstados(afnd);
	num_simbolos = AFNDNumSimbolos(afnd);
	estados_afd = (struct Estado *) malloc(sizeof(struct Estado));

	/*Creamos el nuevo estado inicial*/
	i = AFNDIndiceEstadoInicial(afnd);
	estados_afd[0].componentes = (int*) calloc(num_estados_afnd, sizeof(int));
	estados_afd[0].componentes[i] = 1;
	estados_afd[0].tipo = INICIAL;

	/*Miramos las transiciones lambda desde el inicial*/
	for (j = 0; j < num_estados_afnd; j++) {
		if (AFNDCierreLTransicionIJ(afnd, i, j) && i != j) {
			estados_afd[0].componentes[j] = 1;
			tipo = AFNDTipoEstadoEn(afnd, j);
			if (tipo == INICIAL_Y_FINAL || tipo == FINAL)
				estados_afd[0].tipo = INICIAL_Y_FINAL;
		}
	}

	/*Asignamos el nombre al nuevo estado inicial*/
	nombre_temp = nombreParaEstado(afnd, estados_afd[0].componentes, num_estados_afnd);
	estados_afd[0].nombre = (char *) malloc((strlen(nombre_temp) + 1) * sizeof(char));
	strcpy(estados_afd[0].nombre, nombre_temp);
	free(nombre_temp);

	/*Creo array para marcar a 1 los destinos a los que llego con un simbolo desde un estado*/
	array_destinos = (int*) calloc(num_estados_afnd, sizeof(int));

	/*Por cada fila (estado) que de los que creamos en la tabla nueva*/
	for (estado = 0; estado < num_estados_afd; estado++) {
		/*Por cada columna (simbolo) de la tabla*/
		for (j = 0, es_final = 0, hay_destinos = 0; j < num_simbolos; j++) {
			for (k = 0; k < num_estados_afnd; k++)
				array_destinos[k] = 0;
			/*Juntar los destinos a los que llego con ese simbolo desde cada sub-estado*/
			for (i = 0; i < num_estados_afnd; i++) {
				if (estados_afd[estado].componentes[i] == 1) {
					for (k = 0; k < num_estados_afnd; k++) {
						if (AFNDTransicionIndicesEstadoiSimboloEstadof(afnd, i, j, k)) {
							hay_destinos = 1;
							/*Marco con 1 el indice de k en el array*/
							array_destinos[k] = 1;
							if (AFNDTipoEstadoEn(afnd, k) == INICIAL_Y_FINAL || AFNDTipoEstadoEn(afnd, k) == FINAL)
								es_final = 1;
							/*Marco tambien con 1 los destinos lambda desde k*/
							for (l = 0; l < num_estados_afnd; l++) {
								if (AFNDCierreLTransicionIJ(afnd, k, l) && k != l) {
									array_destinos[l] = 1;
									if (AFNDTipoEstadoEn(afnd, l) == INICIAL_Y_FINAL || AFNDTipoEstadoEn(afnd, l) == FINAL)
										es_final = 1;
								}
							}
						}
					}
				}
			}
			/*Si el array de destinos no esta vacio para ese simbolo*/
			if (hay_destinos) {
				nombre_temp = nombreParaEstado(afnd, array_destinos, num_estados_afnd);
				if(strcmp(nombre_temp, "")) {
					/*Si el estado no existe en nuestra tabla del afd, lo creamos*/
					if (!existe_en_afd(nombre_temp, estados_afd, num_estados_afd)) {
						num_estados_afd++;
						estados_afd = (struct Estado*) realloc(estados_afd, num_estados_afd * sizeof(struct Estado));
						crearEstado(&estados_afd[num_estados_afd-1], nombre_temp, array_destinos, es_final, num_estados_afd, num_estados_afnd);
					}
					/*Creamos la transicion para el afd. Desde el estado 'estado', con 'j' voy al estado 'nombre_temp'*/
					num_trans_afd++;
					transiciones_afd = (struct Transicion *) realloc(transiciones_afd, num_trans_afd * sizeof(struct Transicion));
					crearTransicion(&transiciones_afd[num_trans_afd-1], estados_afd[estado].nombre, nombre_temp, AFNDSimboloEn(afnd, j));
				}
				free(nombre_temp);
			}
		}
	}

	/*Creamos el automata finito e introducimos simbolos, estados y transiciones*/
	afd = AFNDNuevo("AFD", num_estados_afd, num_simbolos);
	for (i = 0; i < num_simbolos; i++)
		AFNDInsertaSimbolo(afd, AFNDSimboloEn(afnd, i));
	for (i = 0; i < num_estados_afd; i++)
		AFNDInsertaEstado(afd, estados_afd[i].nombre, estados_afd[i].tipo);
	for (i = 0; i < num_trans_afd; i++)
		AFNDInsertaTransicion(afd, transiciones_afd[i].origen, transiciones_afd[i].simbolo, transiciones_afd[i].destino);

	liberaMemoria(estados_afd, transiciones_afd, array_destinos, num_estados_afd, num_trans_afd);
	return afd;
}
