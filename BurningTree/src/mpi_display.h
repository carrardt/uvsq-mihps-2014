#ifndef _MPI_DISPLAY_H
#define _MPI_DISPLAY_H

#include "mpi_project.h"

/* Initialisation de l'affichage */
void display_init(struct ctx_s *ctx, const char* filename, int domainX, int domainY, int domainZ );

/* Terminaison de l'affichage */
void display_finalize(struct ctx_s *ctx);
/* Fonction permettant de rajouter une image au fichier GIF. Cette fonction est à
 * appeler à chaque pas de temps. X et Y correspondent aux dimensions de l'image à
 * générer, ratio correspond au nombre de pixels associés à une cellule. */
void display_render_step(struct ctx_s *ctx, int *grid,int startX, int startY, int startZ, int sizeX, int sizeY, int sizeZ, int timeStep);

#endif
