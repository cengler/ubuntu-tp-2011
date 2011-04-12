# Makefile grupo 12 dominguez engler
all: hello launch mypid tinyhello universe

hello : hello.o
	gcc -o hello hello.o

hello.o : hello.c
	gcc -c hello.c

launch : launch.o
	gcc -o launch launch.o

launch.o : launch.c
	gcc -c launch.c

mypid : mypid.o
	gcc -o mypid mypid.o

mypid.o : mypid.c
	gcc -c mypid.c

tinyhello : tinyhello.o
	ld tinyhello.o -o tinyhello

tinyhello.o : tinyhello.asm
	nasm -f elf tinyhello.asm

tinyhello_64 : tinyhello_64.o
	ld tinyhello_64.o -o tinyhello_64

tinyhello_64.o : tinyhello_64.asm
	nasm -f elf tinyhello_64.asm

universe : universe.o
	ld universe.o -o universe

universe.o : universe.asm
	nasm -f elf universe.asm

universe_64 : universe_64.o
	ld universe_64.o -o universe_64

universe_64.o : universe_64.asm
	nasm -f elf universe_64.asm

clean:
	rm -f *.o
	rm -f *.zip

zip: clean all
	zip entrega.zip *.c *.asm
	
