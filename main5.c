#include <stdio.h>
#include "afnd.h"
#include "minimiza.h"

int main(int argc, char ** argv) {
  AFND * p_afnd;
  AFND * p_afnd_min;

  p_afnd = AFNDNuevo("af1",6,2);

  AFNDInsertaSimbolo(p_afnd,"0");
  AFNDInsertaSimbolo(p_afnd,"1");

  AFNDInsertaEstado(p_afnd,"q0",INICIAL);
  AFNDInsertaEstado(p_afnd,"q1",NORMAL);
  AFNDInsertaEstado(p_afnd,"q2",FINAL);
  AFNDInsertaEstado(p_afnd,"q3",FINAL);
  AFNDInsertaEstado(p_afnd,"q4",FINAL);
  AFNDInsertaEstado(p_afnd,"q5",NORMAL);

  AFNDInsertaTransicion(p_afnd, "q0", "0", "q1");
  AFNDInsertaTransicion(p_afnd, "q0", "1", "q2");
  AFNDInsertaTransicion(p_afnd, "q1", "0", "q0");
  AFNDInsertaTransicion(p_afnd, "q1", "1", "q3");
  AFNDInsertaTransicion(p_afnd, "q2", "0", "q4");
  AFNDInsertaTransicion(p_afnd, "q2", "1", "q5");
  AFNDInsertaTransicion(p_afnd, "q3", "0", "q4");
  AFNDInsertaTransicion(p_afnd, "q3", "1", "q5");
  AFNDInsertaTransicion(p_afnd, "q4", "0", "q4");
  AFNDInsertaTransicion(p_afnd, "q4", "1", "q5");
  AFNDInsertaTransicion(p_afnd, "q5", "0", "q5");
  AFNDInsertaTransicion(p_afnd, "q5", "1", "q5");

  p_afnd_min = AFNDMinimiza(p_afnd);

  AFNDImprime(stdout,p_afnd_min);
  AFNDADot(p_afnd_min);

  AFNDElimina(p_afnd);
  AFNDElimina(p_afnd_min);

  return 0;
}
