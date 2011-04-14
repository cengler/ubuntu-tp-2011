#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

const int long_size = sizeof(long);

int main(int argc, char* argv[]) {
	
	int child = fork();
	int status;
	
	if (child == -1) { perror("ERROR fork"); return 1; }
	if (child == 0) {

		if (ptrace(PTRACE_TRACEME, 0, NULL, NULL)) {
			perror("ERROR child ptrace(PTRACE_TRACEME, ...)"); exit(1);
		}
		execvp(argv[1], argv+1);

		perror("ERROR child exec(...)"); exit(1);
	} else {
		while(1) {
			if (wait(&status) < 0) { 
				perror("wait"); 
				break; 
			}
			if (WIFEXITED(status)) 
				break; /* Proceso terminado */
				
			int sysno = ptrace(PTRACE_PEEKUSER, child, 4*ORIG_EAX, NULL);
			
			
			if( sysno == 4) {
					
					int ecx = ptrace(PTRACE_PEEKUSER, child, 4*EBX, NULL);
					int largo_texto = ptrace(PTRACE_PEEKUSER, child, 4*EDX, NULL);
					
			}
			ptrace(PTRACE_SYSCALL, child, NULL, NULL); /* contina */
		}
	ptrace(PTRACE_DETACH, child, NULL, NULL);/*Liberamos al hijo*/
}


	return 0;
}

void toUpperCase(pid_t child, long addr, int len) {
	char *laddr;
	int i,j;
	union u {
		long val;
		char[long_size];
	}data;
	i=0;
	j=len / long_size;
	while(i<j) 
	{
		data.val = ptrace(PTRACE_PEEKDATA, child, addr+i*4, NULL);
		i++;
		laddr += long_size;
	}
}
