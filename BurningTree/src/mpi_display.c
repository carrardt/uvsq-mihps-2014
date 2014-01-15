#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "mpi_project.h"
#include "mpi_display.h"

void display_finalize(struct ctx_s *ctx)
{
	++ ctx->timeStep;
}

void display_init(struct ctx_s *ctx, const char* filename, int domainX, int domainY, int domainZ )
{
	ctx->timeStep = 0;
	ctx->domainCount = 0;
	ctx->domainX = domainX;
	ctx->domainY = domainY;
	ctx->domainZ = domainZ;
	strncpy(ctx->filename,filename,255);
}

void display_render_step(struct ctx_s *ctx, int *grid,int startX, int startY, int startZ, int sizeX, int sizeY, int sizeZ, int timeStep)
{
	char fileName[256];

	if( timeStep > ctx->timeStep )
	{
		ctx->timeStep = timeStep;
		ctx->domainCount = 0;
	}

	snprintf(fileName,255,"%s_Time%04d_Domain%04d.raw",ctx->filename,timeStep,ctx->domainCount);
	FILE* fp = fopen(fileName,"wb");
	if(fp==0)
	{
		fprintf(stderr,"Impossible d'ouvrir le fichier %s",fileName);
		return;
	}

	if( sizeZ==0 ) sizeZ = 1;
	fwrite(grid,sizeX*sizeY*sizeZ*sizeof(int),1,fp);
	fclose(fp);

	++ ctx->domainCount;
}

