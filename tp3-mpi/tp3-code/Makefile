.PHONY  = all clean
MPICC   = mpicc
CFLAGS += -Wall

EJECUTABLES = tp3

all : $(EJECUTABLES)

tp3 : tp3.c
	$(MPICC) $(CFLAGS) -o tp3 $(^)

clean :
	rm -f $(EJECUTABLES)

