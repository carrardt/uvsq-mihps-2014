#include "mpi_project.h"
#include "mpi_display.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

/* Nombre total d'iteration pour la simulation */
#define MAX_STEPS 600
/* Taille du domaine complet (sans les cellules fantomes) */
#define GRIDSZ 100
/* Taille d'une cellule en pixels */
#define RATIO 3

/*-----------------------------------------------------------
 *  Timers: obtention du temps de l'execution de l'application
 *----------------------------------------------------------*/
double get_timer() {
  struct timeval t;
  gettimeofday(&t, NULL);
  return t.tv_sec * 1000000 + t.tv_usec;
}

double diff_time (double end, double start) {
  return end - start;
}

void print_time(struct ctx_s *ctx, double runtime) {
  double runtime_in_seconds = runtime / 1000000;
  fprintf(stderr, "[%d] Temps de l'execution : %.2f s\n", ctx->rank, runtime_in_seconds);
}

/*-----------------------------------------------------------
 *  Mise à jour de la cellule courante
 *----------------------------------------------------------*/
int update_self(struct ctx_s *ctx, int x, int y, int nb_burning) {
  int update = 0;

  if (ctx->grid[x][y] == tree) {
    if (nb_burning > 0) {
      ctx->grid_next[x][y] = burning;
      update++;
    } else {
      ctx->grid_next[x][y] = tree;
    }
  } else if (ctx->grid[x][y] == burning) {
      ctx->grid_next[x][y] = burned;
      update++;
  } else if (ctx->grid[x][y] == burned) {
      ctx->grid_next[x][y] = burned;
  }
  return update;
}

void update_neighbor(struct ctx_s *ctx, int x, int y, int *nb_burning) {
  if (ctx->grid[x][y] == burning) {
    (*nb_burning)++;
  }
}


/*-----------------------------------------------------------
 *  Mise à jour de toutes les cellules de la grille
 *----------------------------------------------------------*/
int update_cells (struct ctx_s *ctx) {
  int x, y;
  int top, bottom, left, right;
  int update = 0;

  for (x=1; x < ctx->X+1; ++x) {
    for (y=1; y < ctx->Y+1; ++y) {
      int nb_burning = 0;
      left    = x - 1;
      right   = x + 1;
      top     = y - 1;
      bottom  = y + 1;

      update_neighbor(ctx, left,  y, &nb_burning);
      update_neighbor(ctx, right, y, &nb_burning);
      update_neighbor(ctx, x,     top, &nb_burning);
      update_neighbor(ctx, x,     bottom, &nb_burning);
      update += update_self(ctx, x, y, nb_burning);
    }
  }
  return update;
}

/*-----------------------------------------------------------
 *  Initialization aleatoire de la grille (on parcourt toute
 *  la grille. Pour chaque cellule de la grille, il y a une probabilite
 *  p=0.6 qu'un arbre s'y trouve.
 *----------------------------------------------------------*/
static void randomize_grid(struct ctx_s *ctx, int **grid) {
  int x, y, r;

  srand (time(NULL) * ctx->rank);

  for (x=1; x < ctx->X+1; ++x) {
    for (y=1; y < ctx->Y+1; ++y) {
      /* Random pour determiner si il y a un arbre */
      r = rand() % 100;
      if (r <= 60) {
        grid[x][y] = tree;
      }
    }
  }
}


/*-----------------------------------------------------------
 *  Initialisation d'une grille
 *----------------------------------------------------------*/
static int ** init_grid(struct ctx_s* ctx) {
  int x;
  int **grid;

  grid = malloc( ((ctx->X+2) * sizeof(int*)) );

  for (x=0; x < ctx->X+2; ++x) {
    grid[x] = malloc ( (ctx->Y+2) * sizeof(int));
    /* Initialise toutes la grille à 0 */
    memset(grid[x], 0, (ctx->Y+2) * sizeof(int));
  }
  return grid;
}


/*-----------------------------------------------------------
 *  Recopie de la grille sans les cellules fantômes et de façon à
 *  avoir toute la grille contigue en memoire.
 *----------------------------------------------------------*/
int * recopy_grid(struct ctx_s* ctx, int **grid) {
  int x, y, i=0;
  int * grid_recopy;

  grid_recopy = malloc(ctx->X * ctx->Y * sizeof(int));

  for (x=1; x < ctx->X+1; ++x) {
    for (y=1; y < ctx->Y+1; ++y) {
      grid_recopy[i] = grid[x][y];
      i++;
    }
  }
  return grid_recopy;
}


/*-----------------------------------------------------------
 *  Fonction MPI: initialization du MPI
 *----------------------------------------------------------*/
void initalize_mpi(int argc, char** argv, struct ctx_s *ctx)
{
  /* A completer en MPI */
  ctx->rank = 0;
  ctx->size = 1;
}


/*-----------------------------------------------------------
 *  Fonction MPI: echange des cellules fantomes
 *----------------------------------------------------------*/
void exchange_ghost_cells(struct ctx_s *ctx)
{
  /* A completer en MPI */
 }


/*-----------------------------------------------------------
 *  Fonction MPI: terminaise du MPI
 *----------------------------------------------------------*/
void finalize_mpi(struct ctx_s *ctx)
{
  /* A completer en MPI */
}


/*-----------------------------------------------------------
 * Fonction MPI: mise en forme de la grille et ecriture sur disque
 * de la sortie
 *----------------------------------------------------------*/
void gather_grid(struct ctx_s * ctx, int * recopy)
{
  static int timeStep = 0;

  /* A completer en MPI */
  display_render_step(ctx, recopy, /*départ du sous-domaine*/ 0,0,0, /*taille du sous-domaine*/ GRIDSZ,GRIDSZ,0, /*pas de temps*/ timeStep++ );

  free(recopy);
}


/*-----------------------------------------------------------
 *  Fonction MPI: verification des conditions finales de la simulation.
 *----------------------------------------------------------*/
int check_final_conditions (struct ctx_s * ctx, int nb_updated_cells)
{
  /* A completer en MPI */
  return 0;
}


/*-----------------------------------------------------------
 * Programme principal
 *----------------------------------------------------------*/
int main (int argc, char **argv) {
  struct ctx_s ctx;
  int i;
  int *recopy;
  double timer_start, timer_end;
  double runtime;
  char output[256];

  /* Initialisation MPI */
  initalize_mpi(argc, argv, &ctx);

  /* Initialisation de la taille du domain (grille carree) */
  ctx.X = GRIDSZ / ctx.size;
  ctx.Y = GRIDSZ;
  ctx.grid = init_grid(&ctx);
  randomize_grid(&ctx, ctx.grid);
  ctx.grid_next = init_grid(&ctx);
  fprintf(stderr, "Starting task %d (%dx%d)\n", ctx.rank, ctx.X, ctx.Y);

  /* Initialisation affichage */
  sprintf(output, "output/Rank%d", ctx.rank);
  display_init(&ctx, output, GRIDSZ,GRIDSZ,0 ); // taille 0 pour la 3ieme dimension signifie une grille 2D

  /* Mettre la case au milieu du sous-domaine de la tache 0 en feu ! */
  if (ctx.rank == 0) {
    ctx.grid[(ctx.X+2)/2][(ctx.Y+2)/2] = burning;
  }

  recopy = recopy_grid(&ctx, ctx.grid);
  gather_grid(&ctx, recopy);

  timer_start = get_timer();
  for (i=0; i < MAX_STEPS; ++i) {
    int **swap;
    /* Nombre de cellules mises à jour depuis le pas
     * de temps precedent */
    int nb_updated_cells;
    /* Si la simulation est terminee */
    int leave;

    exchange_ghost_cells(&ctx);

    nb_updated_cells = update_cells(&ctx);

    leave = check_final_conditions(&ctx, nb_updated_cells);
    if (leave) break;

    swap = ctx.grid;
    ctx.grid = ctx.grid_next;
    ctx.grid_next = swap;

    recopy = recopy_grid(&ctx, ctx.grid);
    gather_grid(&ctx, recopy);
    if (ctx.rank == 0 && i%10 == 0) {
      fprintf(stderr, "..[%d]", i);
    }
  }
  timer_end = get_timer();

  if (ctx.rank == 0) {
    fprintf(stderr, "\n");
  }

  display_finalize(&ctx);
  runtime = diff_time(timer_end, timer_start);
  print_time (&ctx, runtime);

  /* Fin de la simulation */
  finalize_mpi(&ctx);
  exit(0);
}
