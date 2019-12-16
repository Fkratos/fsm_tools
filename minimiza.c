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
  if ((t1 == INICIAL && t2 == NORMAL) || (t2 == INICIAL && t1 == NORMAL)) return 0;
  if ((t1 == INICIAL_Y_FINAL && t2 == FINAL) || (t2 == INICIAL_Y_FINAL && t1 == FINAL)) return 0;
  if (t1 != t2) return 1;
  return 0;
}


/**
 * Genera un nombre para un nuevo estado dados sus estados componentes
 */
char *nombreParaEstadoPeroNo(AFND *afnd, int *componentes, int num_estados_afnd) {

	char *nombre;
	int i;

	nombre = (char *) malloc(MAX_NAME * sizeof(char));
	strcpy(nombre, "");
	for(i = 0; i < num_estados_afnd; i++) {
		if(componentes[i] == 0) {
			if (!strstr(nombre, AFNDNombreEstadoEn(afnd, i)))
				strcat(nombre, AFNDNombreEstadoEn(afnd, i));
		}
	}
	return nombre;
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
  AFND *afmin = NULL, *afnuevo = NULL;
  struct Estado *estados_afmin = NULL, estado_dest;
  int **son_distinguibles = NULL, num_estados_afmin = 0;
  int he_cambiado = 1, num_estados, i, j, k, s, d1, d2, es_final = 0, t1, t2;
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
    for (j = 0; j < num_estados; j++) {
      t1 = AFNDTipoEstadoEn(afmin, i);
      t2 = AFNDTipoEstadoEn(afmin, j);
      // REVISA ESTO. FINAL=INICIAL_Y_FINAL?. INICIAL=NORMAL? Funcion arriba
      if (son_tipos_distintos(t1, t2) == 1) {
        printf("\n\t%d(%d) y %d(%d) SON DE TIPO DISTINTO\n", i, t1, j, t2);
        son_distinguibles[i][j] = 1;
        son_distinguibles[j][i] = 1;
      }
      else printf("\n\t%d(%d) y %d(%d) SON DE TIPO IGUAL\n", i, t1, j, t2);
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
    nombre_temp = nombreParaEstadoPeroNo(afmin, son_distinguibles[i], num_estados);
    if (!existe_en_afd(nombre_temp, estados_afmin, num_estados_afmin)) {
      printf("%s\n", nombre_temp);
      num_estados_afmin++;
      printf("ESTADOS DEL NUEVO: %d\n", num_estados_afmin);
      estados_afmin = (struct Estado *)realloc(estados_afmin, num_estados_afmin * sizeof(struct Estado));
      crearEstado(&estados_afmin[num_estados_afmin - 1], nombre_temp, son_distinguibles[i], es_final, num_estados_afmin, num_estados);
    }
  }

  printf("AF NUEVO CON %d ESTADOS Y %d SIMBOLOS\n", num_estados_afmin, AFNDNumSimbolos(afmin));
  afnuevo = AFNDNuevo("AFMin", num_estados_afmin, AFNDNumSimbolos(afmin));
  for (i = 0; i < AFNDNumSimbolos(afmin); i++)
      AFNDInsertaSimbolo(afnuevo, AFNDSimboloEn(afmin, i));
  for (i = 0; i < num_estados_afmin; i++)
      AFNDInsertaEstado(afnuevo, estados_afmin[i].nombre, estados_afmin[i].tipo);

  for (i = 0; i < num_estados_afmin; i++) {
      j = estados_afmin[i].componentes[0];
      for (s = 0; s < AFNDNumSimbolos(afmin); s++) {
          d1 = indice_estado_destino(afmin, j, s);
          for (k = 0; k < num_estados_afmin; k++) {
              if (estados_afmin[i].componentes[d1] == 1)
                estado_dest = estados_afmin[i];
          }
          printf("DE %s A %s CON %s\n", estados_afmin[i].nombre, estado_dest.nombre, AFNDSimboloEn(afmin, s));
          AFNDInsertaTransicion(afnuevo, estados_afmin[i].nombre, AFNDSimboloEn(afmin, s), estado_dest.nombre);
      }
  }

  /*OJO!! CREAR UN NUEVO AFD, ESTAS DEVOLVIENDO EL DEVUELTO POR TRANSFORMA*/
  /*OJO! LIBERAR Y DEVOLVER UN AF CON LOS ESTADOS NUEVOS INSERTADOS Y LAS TRANSICIONES*/
  for (i = 0; i < num_estados_afmin; i++) {
      free(estados_afmin[i].nombre);
      free(estados_afmin[i].componentes);
  }
  free(estados_afmin);

  for (i = 0; i < num_estados; i++) {
    free(son_distinguibles[i]);
  }
  free(son_distinguibles);
  return afnuevo;
}
