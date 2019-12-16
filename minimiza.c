#include "minimiza.h"

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


int son_tipos_distintos(int t1, int t2) {
  if (t1 == t2) return 0;
  if ((t1 == INICIAL && t2 == NORMAL) || (t2 == INICIAL && t1 == NORMAL)) return 0;
  if ((t1 == INICIAL_Y_FINAL && t2 == FINAL) || (t2 == INICIAL_Y_FINAL && t1 == FINAL)) return 0;
  return 1;
}


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
  AFND *afmin = NULL;
  struct Estado *estados_afmin = NULL;
  struct Transicion *transiciones_afmin = NULL;
  int **son_distinguibles = NULL, num_estados_afmin = 0;
  int he_cambiado = 1, num_estados, i, j, s, d1, d2, es_final = 0, t1, t2;
  char *nombre_temp = NULL;

  /*Al hacer transforma se descartan los estados inaccesibles*/
  afmin = AFNDTransforma(afnd);

  num_estados = AFNDNumEstados(afmin);
  estados_afmin = (struct Estado *)malloc(sizeof(struct Estado));

  son_distinguibles = (int **)calloc(num_estados, sizeof(int *));
  for (i = 0; i < num_estados; i++) {
    son_distinguibles[i] = (int *)calloc(num_estados, sizeof(int));
  }

  /*Si dos estados son de tipo distinto, son distinguibles*/
  for (i = 0; i < num_estados; i++) {
    for (j = i + 1; j < num_estados; j++) {
      t1 = AFNDTipoEstadoEn(afmin, i);
      t2 = AFNDTipoEstadoEn(afmin, j);
      // REVISA ESTO. FINAL=INICIAL_Y_FINAL?. INICIAL=NORMAL? Funcion arriba
      if (t1 != t2) {
        son_distinguibles[i][j] = 1;
        son_distinguibles[j][i] = 1;
      }
    }
  }
  print_distinguibles(afmin, son_distinguibles);

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
              printf("q%d y q%d? \tq%d --(%s)-> q%d.  \tq%d --(%s)-> q%d.\n", i, j, i, AFNDSimboloEn(afmin, s), d1, j, AFNDSimboloEn(afmin, s), d2);
              if (d1 >= 0 && d2 >= 0 && son_distinguibles[d1][d2] && !son_distinguibles[i][j]) {
                printf("   -> q%d y q%d distinguibles. Anotando\n\n", i, j);
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
  print_distinguibles(afmin, son_distinguibles);

  for (i = 0; i < num_estados; i++) {
    nombre_temp = nombreParaEstado(afmin, son_distinguibles[i], num_estados);
    if (!existe_en_afd(nombre_temp, estados_afmin, num_estados_afmin)) {
      printf("%s\n", nombre_temp);
      num_estados_afmin++;
      estados_afmin = (struct Estado *)realloc(estados_afmin, num_estados_afmin * sizeof(struct Estado));
      crearEstado(&estados_afmin[num_estados_afmin - 1], nombre_temp, son_distinguibles[i], es_final, num_estados_afmin, num_estados);
    }
  }

  /*OJO!! CREAR UN NUEVO AFD, ESTAS DEVOLVIENDO EL DEVUELTO POR TRANSFORMA*/
  /*OJO! LIBERAR Y DEVOLVER UN AF CON LOS ESTADOS NUEVOS INSERTADOS Y LAS TRANSICIONES*/
  return afmin;
}
