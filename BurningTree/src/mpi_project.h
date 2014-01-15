#ifndef _MPI_PROJECT_H
#define _MPI_PROJECT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


enum cell_type {
  empty = 0,
  tree = 1,
  burning = 2,
  burned = 3,
};

struct ctx_s {
  int X;
  int Y;
  int **grid;
  int **grid_next;
  /* For MPI */
  int rank;
  int size;
  int *reduce_buffer;

  /* Pour l'interface d'affichage.
   * Ne doit pas être modifié ! */
  int timeStep;
  int domainCount;
  int domainX, domainY, domainZ;
  /* Nom du fichier de sortie */
  char filename[256];
};
#endif

