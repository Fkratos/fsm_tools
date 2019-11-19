#include "transforma.h"

struct Estado {
	char *nombre;
	int *lista_actual;
	int tipo;
};

struct Transicion {
	char *ini;
	char *fin;
	char *simbolo;
};


/**
 *
 */
int in(char *nombre, struct Estado * estados, int len) {
	int i;
	for (i = 0; i < len; i++)
		if (!strcmp(nombre, estados[i].nombre))
			return 1;
	return 0;
}


/**
 * Libera las listas de estados y transiciones
 */
void free_the_nip(struct Estado *new_estados, struct Transicion *new_trans,
		int *lista_estados, int num_estados_afd, int num_trans) {

	int i;
	for (i = 0; i < num_estados_afd; i++) {
		free(new_estados[i].nombre);
		free(new_estados[i].lista_actual);
	}
	free(new_estados);
	for (i = 0; i < num_trans; i++) {
		free(new_trans[i].ini);
		free(new_trans[i].fin);
		free(new_trans[i].simbolo);
	}
	free(new_trans);
	free(lista_estados);
}


/**
 * Transforma un AFND en un AFD
 */
AFND * AFNDTransforma(AFND * afnd) {
	AFND *afd = NULL;
	struct Estado *new_estados = NULL;
	struct Transicion *new_trans = NULL;
	int num_estados_afd = 1, num_simbolos, num_trans = 0, num_estados_afnd, final = 0, tipo;
	int i, j, k, l, estado, *lista_estados;
	char *nombre, nombreF[128];

	strcpy(nombreF, "");
	num_estados_afnd = AFNDNumEstados(afnd);
	num_simbolos = AFNDNumSimbolos(afnd);
	new_estados = (struct Estado *) malloc(sizeof(struct Estado));
	new_estados[0].lista_actual = (int*) calloc(num_estados_afnd, sizeof(int));
	lista_estados = (int*) calloc(num_estados_afnd, sizeof(int));

	/*Conseguimos indice del inicial y le damos nombre al estado*/
	i = AFNDIndiceEstadoInicial(afnd);
	new_estados[0].lista_actual[i] = 1;
	strcat(nombreF, AFNDNombreEstadoEn(afnd, i));

	/*Anadimos las transiciones lambda desde el inicial 'i' a otros estados 'j'*/
	for (j = 0; j < num_estados_afnd; j++) {
		if (AFNDCierreLTransicionIJ(afnd, i, j) && i != j) {
			new_estados[0].lista_actual[j] = 1;
			/*Anadimos el nombre del estado j al nombre del nuevo estado*/
			strcat(nombreF, AFNDNombreEstadoEn(afnd, j));
			tipo = AFNDTipoEstadoEn(afnd, j);
			if (tipo == INICIAL_Y_FINAL || tipo == FINAL)
				final = 1;
		}
	}
	/*Asignamos el nombre y tipo obtenidos al nuevo estado inicial*/
	new_estados[0].nombre = (char *) malloc((strlen(nombreF) + 1) * sizeof(char));
	strcpy(new_estados[0].nombre, nombreF);
	if (final || AFNDTipoEstadoEn(afnd, i) == INICIAL_Y_FINAL)
		new_estados[0].tipo = INICIAL_Y_FINAL;
	else
		new_estados[0].tipo = INICIAL;

	strcpy(nombreF, "");
	/*Iteramos en el conjunto de estados que forma el nuevo estado inicial y
	 * vemos sus transiciones*/
	for (estado = 0; estado < num_estados_afd; estado++) {
		for (j = 0; j < num_simbolos; j++) {
			for (k = 0; k < num_estados_afnd; k++)
				lista_estados[k] = 0;
			for (i = 0; i < num_estados_afnd; i++) {
				if (new_estados[estado].lista_actual[i] == 1) {
					for (k = 0; k < num_estados_afnd; k++) {
						/*Vemos si existe una transicion i, j, k*/
						if (AFNDTransicionIndicesEstadoiSimboloEstadof(afnd, i, j, k)) {
							/*Metemos el nombre del estado en la nueva cadena*/
							lista_estados[k] = 1;
							nombre = AFNDNombreEstadoEn(afnd, k);
							if (!strstr(nombreF, nombre))
								strcat(nombreF, nombre);

							/*Miramos si el tipo es final*/
							tipo = AFNDTipoEstadoEn(afnd, k);
							if (tipo == INICIAL_Y_FINAL || tipo == FINAL)
								final = 1;

							/*Miramos las transiciones lambda desde k*/
							for (l = 0; l < num_estados_afnd; l++) {
								if (AFNDCierreLTransicionIJ(afnd, k, l) && k != l) {
									nombre = AFNDNombreEstadoEn(afnd, l);
									if (!strstr(nombreF, nombre))
										strcat(nombreF, nombre);
									lista_estados[l] = 1;

									/*Miramos si el tipo es final*/
									tipo = AFNDTipoEstadoEn(afnd, l);
									if (tipo == INICIAL_Y_FINAL || tipo == FINAL)
										final = 1;
								}
							}
						}
					}
				}
			}
			/*Creamos el nuevo estado y la nueva transicion*/
			if (strcmp("", nombreF)) {
				if (!in(nombreF, new_estados, num_estados_afd)) {
					num_estados_afd++;
					new_estados = (struct Estado*) realloc(new_estados, num_estados_afd * sizeof(struct Estado));
					new_estados[num_estados_afd - 1].lista_actual = (int*) calloc(num_estados_afnd, sizeof(int));
					for (l = 0; l < num_estados_afnd; l++) {
						new_estados[num_estados_afd - 1].lista_actual[l] = lista_estados[l];
						lista_estados[l] = 0;
					}

					new_estados[num_estados_afd - 1].nombre = (char *) malloc(sizeof(char)*(strlen(nombreF) + 1));
					strcpy(new_estados[num_estados_afd - 1].nombre, nombreF);
					if (final)
						new_estados[num_estados_afd - 1].tipo = FINAL;
					else
						new_estados[num_estados_afd - 1].tipo = NORMAL;
				}

				num_trans++;
				new_trans = (struct Transicion *) realloc(new_trans, num_trans * sizeof(struct Transicion));
				new_trans[num_trans - 1].ini = (char*) malloc(sizeof(char)*(strlen(new_estados[estado].nombre) + 1));
				strcpy(new_trans[num_trans - 1].ini, new_estados[estado].nombre);
				new_trans[num_trans - 1].fin = (char*) malloc(sizeof(char)*(strlen(nombreF) + 1));
				strcpy(new_trans[num_trans - 1].fin, nombreF);
				nombre = AFNDSimboloEn(afnd, j);
				new_trans[num_trans - 1].simbolo = (char*) malloc(sizeof(char)*(strlen(nombre) + 1));
				strcpy(new_trans[num_trans - 1].simbolo, nombre);
				strcpy(nombreF, "");
			}
		}
	}

	/*Creamos el automata finito*/
	afd = AFNDNuevo("finito", num_estados_afd, num_simbolos);
	/*Metemos los simbolos, estados y transiciones*/
	for (i = 0; i < num_simbolos; i++)
		AFNDInsertaSimbolo(afd, AFNDSimboloEn(afnd, i));
	for (i = 0; i < num_estados_afd; i++)
		AFNDInsertaEstado(afd, new_estados[i].nombre, new_estados[i].tipo);
	for (i = 0; i < num_trans; i++)
		AFNDInsertaTransicion(afd, new_trans[i].ini, new_trans[i].simbolo, new_trans[i].fin);

	free_the_nip(new_estados, new_trans, lista_estados, num_estados_afd, num_trans);
	return afd;
}
