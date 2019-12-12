#include "minimiza.h"

int indice_estado_destino(AFND *afd, int i_origen, int i_simbolo) {
  int dest;
  for (dest = 0; dest < AFNDNumEstados(afd); dest++) {
    if (AFNDTransicionIndicesEstadoiSimboloEstadof(afd, i_origen, dest, i_simbolo))
      return dest;
  }
  return -1;
}

/**
 * Minimiza un AFND
 */
AFND * AFNDMinimiza(AFND *afnd) {
  AFND *afmin = NULL;
  struct Estado *estados_afmin = NULL;
	struct Transicion *transiciones_afmin = NULL;
  int **son_distinguibles = NULL, num_estados_afmin = 0;
  int he_cambiado = 1, num_estados, i, j, s, d1, d2, es_final = 0;
  char *nombre_temp = NULL;

  afmin = AFNDTransforma(afnd);
  num_estados = AFNDNumEstados(afmin);
  estados_afmin = (struct Estado *) malloc(sizeof(struct Estado));

  son_distinguibles = (int **) calloc(num_estados, sizeof(int *));
  for (i = 0; i < num_estados; i++) {
    son_distinguibles[i] = (int *) calloc(num_estados, sizeof(int));
  }

  /*Si dos estados son de tipo distinto o llegan a distinto tipo de estados, son distinguibles*/
  for(i = 0; i < num_estados; i++) {
    for(j = i+1; j < num_estados; j++) {
      if(AFNDTipoEstadoEn(afmin, i) != AFNDTipoEstadoEn(afmin, j)) {
        son_distinguibles[i][j] = 1;
        son_distinguibles[j][i] = 1;
      }
    }
  }
  while (he_cambiado == 1) {
    he_cambiado = 0;
    for(i = 0; i < num_estados; i++) {
      for(j = i+1; j < num_estados; j++) {
        for(s = 0; s < AFNDNumSimbolos(afmin); s++) {
          d1 = indice_estado_destino(afmin, i, s);
          d2 = indice_estado_destino(afmin, j, s);
          if (son_distinguibles[d1][d2]) {
            son_distinguibles[i][j] = 1;
            son_distinguibles[j][i] = 1;
            he_cambiado = 1;
          }
        }
      }
    }
  }
  printf("Enhorabuena. Has salido, quien lo diria eh. Suave.\n");

  for(i = 0; i < num_estados; i++) {
    nombre_temp = nombreParaEstado(afmin, son_distinguibles[i], num_estados);
    if (!existe_en_afd(nombre_temp, estados_afmin, num_estados_afmin)) {
      printf("%s", nombre_temp);
      num_estados_afmin++;
      estados_afmin = (struct Estado*) realloc(estados_afmin, num_estados_afmin * sizeof(struct Estado));
      crearEstado(&estados_afmin[num_estados_afmin-1], nombre_temp, son_distinguibles[i], es_final, num_estados_afmin, num_estados);
    }
  }
  /*OJO! LIBERAR Y DEVOLVER UN AF CON LOS ESTADOS NUEVOS INSERTADOS Y LAS TRANSICIONES*/
  return afmin;
}
