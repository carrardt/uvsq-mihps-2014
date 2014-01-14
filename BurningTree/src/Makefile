CC=gcc
CFLAGS=-O2 -g -mtune=native -march=native -Wall
LDFLAGS=
RM=/bin/rm
PROGRAM=BurningTree

all: ${PROGRAM}

${PROGRAM}: mpi_display.o mpi_project.o
	${CC} mpi_display.o mpi_project.o -o ${PROGRAM} ${LDFLAGS}

mpi_display.o: mpi_display.c mpi_display.h
	${CC} ${CFLAGS} -o mpi_display.o -c mpi_display.c

mpi_project.o: mpi_project.c mpi_project.h mpi_display.h
	${CC} ${CFLAGS} -o mpi_project.o -c mpi_project.c

clean:
	$(RM) -f mpi_project.o mpi_display.o ${PROGRAM}

