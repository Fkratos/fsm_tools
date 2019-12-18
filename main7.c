#include <stdio.h>
#include "afnd.h"
#include "minimiza.h"

int main(int argc, char ** argv) {
  AFND * p_afnd;
  AFND * p_afnd_min;

  p_afnd = AFNDNuevo("af1",5,2);

  AFNDInsertaSimbolo(p_afnd,"0");
  AFNDInsertaSimbolo(p_afnd,"1");

  AFNDInsertaEstado(p_afnd,"A",INICIAL);
  AFNDInsertaEstado(p_afnd,"B",NORMAL);
  AFNDInsertaEstado(p_afnd,"C",NORMAL);
  AFNDInsertaEstado(p_afnd,"D",NORMAL);
  AFNDInsertaEstado(p_afnd,"E",FINAL);

  AFNDInsertaTransicion(p_afnd, "A", "1", "C");
  AFNDInsertaTransicion(p_afnd, "A", "0", "B");
  AFNDInsertaTransicion(p_afnd, "B", "1", "D");
  AFNDInsertaTransicion(p_afnd, "B", "0", "B");
  AFNDInsertaTransicion(p_afnd, "C", "1", "C");
  AFNDInsertaTransicion(p_afnd, "C", "0", "B");
  AFNDInsertaTransicion(p_afnd, "D", "1", "E");
  AFNDInsertaTransicion(p_afnd, "D", "0", "B");
  AFNDInsertaTransicion(p_afnd, "E", "0", "B");
  AFNDInsertaTransicion(p_afnd, "E", "1", "C");

  p_afnd_min = AFNDMinimiza(p_afnd);

  AFNDImprime(stdout,p_afnd_min);
  AFNDADot(p_afnd_min);

  AFNDElimina(p_afnd);
  AFNDElimina(p_afnd_min);

  return 0;
}
