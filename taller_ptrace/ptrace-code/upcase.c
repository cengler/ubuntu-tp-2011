#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char* argv[]) {
	
	/* Salvamos el File Descriptors de la salida standard. */
	int std_out = dup(1);
		
	/* CREO OTRO PROCESO */
	int child = fork();
	if (child == -1) { 
		perror("ERROR fork"); 
		return 1; 
	}
	
	int pdes[2];
	pipe(pdes);
	
	if (child == 0) {
		/* HIJO */
		
		/* CIERRO EL EXTREMO DEL PIPE NO USADO*/
		close(pdes[0]); 
		
		/* Redirigimos la salida estandar. */
		dup2(pdes[1], 1);
		
		/* Ejecutamos el comando recibido. */
		execvp(argv[1], argv+1);
		
		/* Si vuelve de exec() hubo un error */
		perror("ERROR child exec(...)"); 
		exit(1);
		
	} else {
		/* PADRE */
		
		/* CIERRO EL EXTREMO DEL PIPE NO USADO*/
		close(pdes[1]); 
		
		dup2(std_out, pdes[0]);
		
	}

	return 0;
}
