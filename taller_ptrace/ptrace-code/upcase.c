#include <stdlib.h>			// std lib
#include <sys/ptrace.h>		// ptrace
#include <sys/reg.h> 		// REG EAX, EBX ... def
#include <sys/syscall.h>	// Sys_write ... def
#include <unistd.h>			// STDOUT_FILENO

#define WORD_SIZE 4

void editBloquedProcess(pid_t child) {
	
	/* Obtenemos el numero de llamada al sistema operativo */
	int sysno = ptrace(PTRACE_PEEKUSER, child, 4*ORIG_EAX, NULL);
	
	/* Si es una llamada de escritura */
	if( sysno == SYS_write) {	
		
		/* Obtenemos el destino de la escritura */
		int destino = ptrace(PTRACE_PEEKUSER, child, 4*EBX, NULL);
		
		/* Si es la salida estandar */
		if( destino == STDOUT_FILENO) {
			
			/* Obtenemos la direccion de memoria 
			 * que contiene el texto a imprimir */
			int addr = ptrace(PTRACE_PEEKUSER, child, 4*ECX, NULL);
			
			/* Obtenemos el largo del texto a imprimir */
			int len = ptrace(PTRACE_PEEKUSER, child, 4*EDX, NULL);	
			
			/* Creamos una estructura de tipo union 
			 * que permite solo un parametro a la vez
			 * Nos va a facilitar las conversiones de long a char[] */
			union u {
				long val;
				char chars[WORD_SIZE];
			}data;
			
			/* Recorremos los datos en la memoria 
			 * del hijo por 'words' */
			int i;
			for(i = 0; i<len/WORD_SIZE+1; i++) {
				
				/* Obtenemos el i-esimo word de los datos */
				data.val = ptrace(PTRACE_PEEKDATA, 
								child, addr+WORD_SIZE*i, NULL);
				
				/* Pasamos a MAYUSCULAS los 4 caracteres leidos */
				int j;
				for(j = 0; j<WORD_SIZE; j++)	
					data.chars[j] = toupper(data.chars[j]);
				
				/* Escribimos en la memoria del hijo 
				 * los datos actualizados */
				ptrace(PTRACE_POKEDATA, 
							child, addr+WORD_SIZE*i, data.val);
			}		
		}
	}
}

int main(int argc, char* argv[]) {

	/* Verificamos que nos envien un comando para ejecutar */
	if(argc < 2) {
		perror("Uso: ./upcase commando [argumentos ...]\n"); 
		return 1; 
	}
	
	/* Hacemos fork, para tener un proceso controlador
	 * y el que nos pasan por parametro */
	int child = fork();
	
	/* Si hubo error en el fork */
	if (child == -1) { 
		perror("ERROR fork"); 
		return 1; 
	}
	
	if (child == 0) { /* Si ej el hijo */

		/* Lo preparo para ser monitoreado */
		if (ptrace(PTRACE_TRACEME, 0, NULL, NULL)) {
			perror("ERROR child ptrace(PTRACE_TRACEME, ...)"); exit(1);
		}
		
		/* Ejecuto el programa pasado por parametro */
		execvp(argv[1], argv+1);
		
		/* Si sigue despues del exec es porque hubo un error */
		perror("ERROR child exec(...)"); 
		exit(1);	
	
	} else { /* Si es el padre */
		
		/* Mientras que el proceso no salga, ciclamos infinitamente */
		while(1) {
			
			/* El proceso padre se bloquea hasta que 
			 * el hijo tenga un evento */
			int status;
			if (wait(&status) < 0) { 
				perror("wait"); 
				break; 
			}
			
			/* Si el hijo finalizo corto el ciclo infinito */
			if (WIFEXITED(status)) 
				break; /* Proceso terminado */
			
			/* Verificamos y modificamos la memoria 
			 * y el hijo bloqueado */
			editBloquedProcess(child);
			
			/* Liberamos al proceso hijo para que 
			 * continue ejecutando */
			ptrace(PTRACE_SYSCALL, child, NULL, NULL);
		}
		
		/* Liberamos al hijo para que continue se ejecucion */
		ptrace(PTRACE_DETACH, child, NULL, NULL);
	}
	return 0;
}
