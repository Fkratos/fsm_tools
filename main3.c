#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "afnd.h"
#include "transforma.h"

int main(int argc, char ** argv)
{

	AFND * p_afnd;
	AFND * afd;

	p_afnd = AFNDNuevo("af3", 8, 2);

	AFNDInsertaSimbolo(p_afnd, "0");
  AFNDInsertaSimbolo(p_afnd, "1");

  AFNDInsertaEstado(p_afnd, "q0", INICIAL);
  AFNDInsertaEstado(p_afnd, "q1", NORMAL);
	AFNDInsertaEstado(p_afnd, "q2", NORMAL);
	AFNDInsertaEstado(p_afnd, "q3", NORMAL);
	AFNDInsertaEstado(p_afnd, "q4", NORMAL);
	AFNDInsertaEstado(p_afnd, "q5", NORMAL);
	AFNDInsertaEstado(p_afnd, "q6", FINAL);
  AFNDInsertaEstado(p_afnd, "q7", NORMAL);

  AFNDInsertaTransicion(p_afnd, "q1", "1", "q5");
  AFNDInsertaTransicion(p_afnd, "q1", "1", "q3");
  AFNDInsertaTransicion(p_afnd, "q2", "0", "q4");
  AFNDInsertaTransicion(p_afnd, "q4", "1", "q6");
	AFNDInsertaTransicion(p_afnd, "q5", "0", "q4");
	AFNDInsertaTransicion(p_afnd, "q5", "0", "q3");

	AFNDInsertaLTransicion(p_afnd, "q0", "q1");
  AFNDInsertaLTransicion(p_afnd, "q0", "q2");
	AFNDInsertaLTransicion(p_afnd, "q5", "q7");
	AFNDInsertaLTransicion(p_afnd, "q7", "q6");
	AFNDCierraLTransicion(p_afnd);

	afd  = AFNDTransforma(p_afnd);
	AFNDImprime(stdout,afd);
	AFNDADot(afd);

	AFNDElimina(afd);
	AFNDElimina(p_afnd);

	return 0;
}
