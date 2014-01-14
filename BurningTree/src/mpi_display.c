#include "mpi_display.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int frame_counter = 0;

void display_finalize(struct ctx_s *ctx)
{
}

void display_init(struct ctx_s *ctx, const char* filename, int domainX, int domainY )
{
	ctx->timeStepCount = 0;
	ctx->domainX = domainX;
	ctx->domainY = domainY;
	strncpy(ctx->filename,255,filename);
	ctx->colorMap[0+0] = 128;
	ctx->colorMap[0+1] = 128;
	ctx->colorMap[0+2] = 128;
	ctx->colorMap[1+0] = 0;
	ctx->colorMap[1+1] = 255;
	ctx->colorMap[1+2] = 0;
	ctx->colorMap[2+0] = 255;
	ctx->colorMap[2+1] = 0;
	ctx->colorMap[2+2] = 0;
	ctx->colorMap[3+0] = 0;
	ctx->colorMap[3+1] = 0;
	ctx->colorMap[3+2] = 0;
}

void display_render_step(struct ctx_s *ctx, int *grid,int debutX, int debutY, int tailleX, int tailleY, int temps)
{
	++ ctx->timeStepCount;

}
