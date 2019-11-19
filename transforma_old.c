#include "transforma.h"
#include <string.h>

/*Convierte AFND de ellos a una tabla*/
char ***convertir_afnd_a_tabla(AFND *afnd) {
	char ***array = NULL;

	num_estados = AFNDNumEstados(afnd);
	num_simbolos = AFNDNumSimbolos(afnd);

	/*array[num_estados * num_simbolos][num_estados]*/
	array = (char ***) calloc(num_estados*num_simbolos, sizeof(char**));
	for(i=0; i<num_estados*num_simbolos; i++) {
		array[i] = (char **) calloc(num_estados+1, sizeof(char*));
		array[i] = NULL;
	}

	for(i=0; i<num_estados; i++) {
		for(j=0, counter=0; j<num_simbolos; j++) {
			for(k=0; j<num_estados; j++) {
				if AFNDTransicionIndicesEstadoiSimboloEstadof(afnd, i, j, k) {
					strcpy(array[i * num_simbolos + j][counter], AFNDNombreEstadoEn(afnd, k));
					counter++;
				}
			}
		}
	}
	return array;
}


/*
Recibe un array de strings.
Los ordena alfabeticamente, los concatena y devuelve el string resultante
*/
char * nombreParaEstado(int *indices_de_estados) {
	return NULL;
}

char * getSimbolos(AFND *afnd) {
	char * alfabeto = NULL;
	for(i=0; i<AFNDNumSimbolos(afnd); i++) {
		/* alfabeto.append AFNDSimboloEn(afnd, i)*/
	}
	return alfabeto;
}

int *getDestinosLambda(AFND *afnd, int origen) {
	int *cadena_lambdas = NULL;
	cadena_lambdas = (int *) calloc(num_estados+1, sizeof(int))

	for(i=0; num_lambda=0; i<num_estados; i++) {
		/*Si hay salto lambda de origen a i*/
		if(AFNDCierreLTransicionIJ(origen, i)==1) {
			cadena_lambdas[num_lambda] = i;
			num_lambda++;
		}
	}
	/*Para parar de leer la cadena, pongo el ultimo a null*/
	cadena_lambdas[num_lambda] = -1;
	return cadena_lambdas;
}

int * getEstadosDestino(AFND *afnd, int origen, char *simbolo, int num_estados) {
	int destinos[num_estados];

	/*Para ESTADO in afnd.estados*/
		/*si AFNDTransicionIndicesEstadoiSimboloEstadof(afnd, origen, simbolo, ESTADO)*/
			/*Meto ESTADO en destinos*/

	return destinos; /*Ojo puntero, array y tal*/
}

AFND * AFNDTransforma(AFND * afnd) {
	AFND * afd = NULL, afnd_cerrado = NULL;
	char ***array = NULL, **array_new = NULL;
	char **destinos_lambda = NULL;

	afnd_cerrado = AFNDCierraLTransicion(afnd);
	if (!afnd_cerrado) {
		printf("Cierre transitivo salio mal\n");
		return NULL;
	}

	array = convertir_afnd_a_tabla(afnd_cerrado);

	/*Obtener el estado inicial del afnd*/
	q0_index = AFNDIndiceEstadoInicial(afnd_cerrado);
	/*Obtener los indices de los estados a los que llego con lambda desde q0*/
	destinos_lambda = getDestinosLambda(afnd_cerrado, q0_index);

	/*TODO Inicializa array_new */

	array_new[0][0] = nombreParaEstado(destinos_lambda);
	for(j=0; j<num_simbolos; j++) {
		for(d=0; destinos_lambda[d] != -1; d++) {
			/*Voy a hacer el array de abajo. Like a champ. bidimensional */
			array_new[0][j] = nombreParaEstado(estadosDestinoQ0Con+ y estadosDestinoQ1Con+)
		}
	}

		q0 = array[q0_index * num_simbolos]
	nombreParaEstado(array[q0_index * num_simbolos + AFNDIndiceDeSimbolo("lambda")])




	/*Obtener los nombres de los estados a los que llego con lambda desde e_inicial_afnd*/
	/*Guardarlos en la tabla_lambdas para generar el nombre del estado inicial nuevo*/
	tabla_lambdas = (char **) calloc(AFNDNumEstados(afnd_cerrado), sizeof(char *));
	for(estado=0, num_lambda=0; estado<AFNDNumEstados(afnd_cerrado); estado++) {
		/*Si hay salto lambda de inicial a estado, o inicial es el propio estado*/
		if(AFNDCierreLTransicionIJ(e_inicial_afnd, estado)==1 || e_inicial_afnd==estado) {
			tabla_lambdas[num_lambda] = AFNDNombreEstadoEn(afnd_cerrado, estado);
			num_lambda++;
		}
	}
	nombre_inicial = nombreParaEstado(tabla_lambdas);
	if(!nombre_inicial) {
		printf("Nombre para estado inicial salio mal\n");
		return NULL;
	}

	/*Crear el nuevo estado inicial con el nombre obtenido*/
	AFNDInsertaEstado(afd, nombre_inicial, INICIAL);
	/*cada estado que yo tengo que comprobar esta en tabla_lambdas[i]*/
	for(i=0; i<num_lambda; i++) {
		/*for(simbolo in getSimbolos)*/
			/*destinos += pillar los estados a los que vas desde tabla_lambdas[i] con simbolo*/
			/*es decir, getEstadosDestino(AFNDIndiceDeEstado(nombre que es tabla_lambdas[i]))*/


	}

	for(estado=0; estado<AFNDNumEstados(afnd_cerrado); estado++)


	for()

	por cada estado E de afnd
		si tiene transiciones lambda #AFNDCierreLTransicionIJ
			por cada estado al que pasa con lambda
				appendearlo a estado E
		meter E al array de estados nuevos

	afd = AFNDNuevo();
	afd.AFNDInsertaEstado(inicial concatenadas las lambda de afnd.inicial)
	afd.AFNDInsertaEstado(otroestado concatenadas las lambda de estado)

}
