CC=gcc
CFLAGS=-O0 -pthread
EJECUS=pthreads

all : $(EJECUS)

pthreads : pthreads.c
	$(CC) $(CFLAGS) -o pthreads $(^)

clean :
	rm -f $(EJECUS)

