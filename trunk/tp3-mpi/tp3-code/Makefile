.PHONY  = all clean try
MPICC   = mpicc
CFLAGS += -Wall

EJECUTABLES = tp3

all : $(EJECUTABLES)

tp3 : tp3.c
	$(MPICC) $(CFLAGS) -o tp3 $(^)

clean :
	rm -f $(EJECUTABLES)

try : tp3
	mpiexec -np 4 ./tp3 2>.try && cat .try
