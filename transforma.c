#include "transforma.h"

/*
Recibe un array de strings.
Los ordena alfabeticamente, los concatena y devuelve el string resultante
*/
char * nombreParaEstado(char **tabla_lambdas) {
	return NULL;
}

char * getSimbolos(AFND *afnd) {
	char * alfabeto = NULL;
	for(i=0; i<AFNDNumSimbolos(afnd); i++) {
		/* alfabeto.append AFNDSimboloEn(afnd, i)*/
	}
	return alfabeto;
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

	afd = AFNDNuevo(char * nombre, int num_estados, int num_simbolos);
	afnd_cerrado = AFNDCierraLTransicion(afnd);
	if (!afnd_cerrado) {
		printf("Cierre transitivo salio mal\n");
		return NULL;
	}
	/*Obtener el estado inicial del afnd*/
	e_inicial_afnd = AFNDIndiceEstadoInicial(afnd_cerrado);

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
