#include "minimiza.h"

/**
 * Devuelve el estado al que se llega dado un origen y un simbolo
 */
int indice_estado_destino(AFND *afd, int i_origen, int i_simbolo) {
	int dest;
	if (!afd || i_origen < 0 || i_simbolo < 0) {
		printf("Error en indice_estado_destino\n");
		return -1;
	}
	for (dest = 0; dest < AFNDNumEstados(afd); dest++) {
		if (AFNDTransicionIndicesEstadoiSimboloEstadof(afd, i_origen, i_simbolo, dest))
			return dest;
	}
return -1;
}


/**
 * Decide si t1 y t2 son tipos distintos (distingue 'finales' de 'no finales')
 */
int son_tipos_distintos(int t1, int t2) {
	if ((t1 == INICIAL && t2 == NORMAL) || (t2 == INICIAL && t1 == NORMAL)) return 0;
	if ((t1 == INICIAL_Y_FINAL && t2 == FINAL) || (t2 == INICIAL_Y_FINAL && t1 == FINAL)) return 0;
	if (t1 != t2) return 1;
	return 0;
}


/**
 * Imprime la tabla 'son_distinguibles' (para debug)
 */
void print_distinguibles(AFND *afd, int **son_distinguibles) {
	int i, j;
	printf("\n      ");
	for (j = 0; j < AFNDNumEstados(afd); j++) {
		if (j < 10) printf("q%d   ", j);
		else printf("q%d  ", j);
	}
	printf("\n----+");
	for (j = 0; j < AFNDNumEstados(afd); j++) {
		printf("-----");
	}
	printf("\n");
	for (i = 0; i < AFNDNumEstados(afd); i++) {
		if (i < 10) printf("q%d  | ", i);
		else printf("q%d | ", i);
		for (j = 0; j < AFNDNumEstados(afd); j++) {
			printf(" %d   ", son_distinguibles[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}


/**
 * Minimiza un AFND
 */
AFND *AFNDMinimiza(AFND *afnd) {

	AFND *afmin = NULL, *afnuevo = NULL;
	struct Estado *estados_afmin = NULL, estado_dest;
	int **son_distinguibles = NULL, num_estados_afmin = 0;
	int he_cambiado = 1, num_estados, i, j, k, s, d1, d2, t1, t2;
	char *nombre_temp = NULL;

	/*Al hacer transforma se descartan los estados inaccesibles*/
	afmin = AFNDTransforma(afnd);

	num_estados = AFNDNumEstados(afmin);
	estados_afmin = (struct Estado *)malloc(sizeof(struct Estado));

	son_distinguibles = (int **)calloc(num_estados, sizeof(int *));
	for (i = 0; i < num_estados; i++)
		son_distinguibles[i] = (int *)calloc(num_estados, sizeof(int));

	/*Si dos estados son de tipo distinto, son distinguibles*/
	for (i = 0; i < num_estados; i++) {
		for (j = 0; j < num_estados; j++) {
			t1 = AFNDTipoEstadoEn(afmin, i);
			t2 = AFNDTipoEstadoEn(afmin, j);
			if (son_tipos_distintos(t1, t2) == 1) {
				son_distinguibles[i][j] = 1;
				son_distinguibles[j][i] = 1;
			}
		}
	}

	/*Si dos estados (a priori no distinguibles) llegan con el mismo simbolo a
	*destinos distinguibles, son distinguibles*/
	while (he_cambiado == 1) {
		he_cambiado = 0;
		for (i = 0; i < num_estados; i++) {
			for (j = i + 1; j < num_estados; j++) {
				if (!son_distinguibles[i][j]) {
					for (s = 0; s < AFNDNumSimbolos(afmin); s++) {
						if (!son_distinguibles[i][j]) {
							d1 = indice_estado_destino(afmin, i, s);
							d2 = indice_estado_destino(afmin, j, s);
							if (d1 >= 0 && d2 >= 0 && son_distinguibles[d1][d2] && !son_distinguibles[i][j]) {
								son_distinguibles[i][j] = 1;
								son_distinguibles[j][i] = 1;
								he_cambiado = 1;
							}
						}
					}
				}
			}
		}
	}

	/*Dada la tabla, creamos los estados: juntamos aquellos que en la fila tengan 0's.
	*Como nombreParaEstado y crearEstado trabajan con 1's, invertimos los valores (1), 
	*llamamos a las funciones y revertimos los valores, dejandolos como estaban (2)*/
	for (i = 0; i < num_estados; i++) {
		for (j = 0; j < num_estados; j++)  /*1*/
			son_distinguibles[i][j] = 1-son_distinguibles[i][j];

		nombre_temp = nombreParaEstado(afmin, son_distinguibles[i], num_estados);
		if (!existe_en_afd(nombre_temp, estados_afmin, num_estados_afmin)) {
			num_estados_afmin++;
			estados_afmin = (struct Estado *)realloc(estados_afmin, num_estados_afmin * sizeof(struct Estado));
			crearEstado(afmin, &estados_afmin[num_estados_afmin - 1], nombre_temp, son_distinguibles[i], num_estados_afmin, num_estados);
		}
		free(nombre_temp);

		for (j = 0; j < num_estados; j++)  /*2*/
			son_distinguibles[i][j] = 1-son_distinguibles[i][j];
	}

	/*Creamos un AF nuevo, insertamos los simbolos e insertamos los estados del array de Estados*/
	afnuevo = AFNDNuevo("afm", num_estados_afmin, AFNDNumSimbolos(afmin));
	for (i = 0; i < AFNDNumSimbolos(afmin); i++)
		AFNDInsertaSimbolo(afnuevo, AFNDSimboloEn(afmin, i));
	for (i = 0; i < num_estados_afmin; i++)
		AFNDInsertaEstado(afnuevo, estados_afmin[i].nombre, estados_afmin[i].tipo);

	/*Para las transiciones de cada estado 'grande', busco (para cada simbolo) el destino desde el primer 
	*componente, obtengo el estado 'grande' que lo contiene, e inserto la transicion hacia el 'grande'*/
	for (i = 0; i < num_estados_afmin; i++) {
		for (j = 0; j < num_estados; j++) {
			if (estados_afmin[i].componentes[j] == 1) 
				break;
		}
		for (s = 0; s < AFNDNumSimbolos(afmin); s++) {
			d1 = indice_estado_destino(afmin, j, s);
			for (k = 0; k < num_estados_afmin; k++) {
				if (estados_afmin[k].componentes[d1] == 1)
					estado_dest = estados_afmin[k];
			}
			AFNDInsertaTransicion(afnuevo, estados_afmin[i].nombre, AFNDSimboloEn(afmin, s), estado_dest.nombre);
		}
	}

	/*Liberamos memoria reservada*/
	for (i = 0; i < num_estados_afmin; i++) {
		free(estados_afmin[i].nombre);
		free(estados_afmin[i].componentes);
	}
	free(estados_afmin);
	for (i = 0; i < num_estados; i++) 
		free(son_distinguibles[i]);
	free(son_distinguibles);
	AFNDElimina(afmin);

	/*Devolvemos el AF minimizado*/
	return afnuevo;
}
