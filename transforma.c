#include "transforma.h"

AFND * AFNDTransforma(AFND * afnd) {
	por cada estado E de afnd
		si tiene transiciones lambda #AFNDCierreLTransicionIJ
			por cada estado al que pasa con lambda
				appendearlo a estado E
		meter E al array de estados nuevos

	afd = AFNDNuevo();
	afd.AFNDInsertaEstado(inicial concatenadas las lambda de afnd.inicial)
	afd.AFNDInsertaEstado(otroestado concatenadas las lambda de estado)

}
