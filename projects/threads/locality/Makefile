# Makefile for matrix routines
# Created by Jerod Weinman, August 2008
# Revised 8 August, 2014

CC=gcc

# Requires compiling with _GNU_SOURCE for RUSAGE_THREAD in getrusage(3) and
# CPU_SET
DEFINES=-D_GNU_SOURCE
FLAGS=-g -Wall

# Compile matrixop.c into an object file that can be linked with other programs
matrixop: matrix.h matrixop.h matrixop.c
	$(CC) $(FLAGS) $(DEFINES) -c matrixop.c

# Compile matrixutil.c into an object file also
matrixutil: matrix.h matrixutil.h matrixutil.c
	$(CC) $(FLAGS) $(DEFINES) -c matrixutil.c	

# Set this target to depend on the previous two and pass object files to link
add: matrixop matrixutil testadd.c
	$(CC) $(FLAGS) $(DEFINES) -o testadd matrixop.o matrixutil.o testadd.c

timemultiply: matrixop matrixutil timemultiply.c
	$(CC) $(FLAGS) $(DEFINES) -pthread -o timemultiply \
	matrixop.o matrixutil.o timemultiply.c

ptimemultiply: matrixop matrixutil ptimemultiply.c
	$(CC) $(FLAGS) $(DEFINES) -pthread -o ptimemultiply \
	matrixop.o matrixutil.o ptimemultiply.c

multiply: matrixop matrixutil testmultiply.c
	$(CC) $(FLAGS) $(DEFINES) -pthread -o testmultiply matrixop.o matrixutil.o \
	testmultiply.c

clean:
	rm -f *.o *~ testadd timemultiply
