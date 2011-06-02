#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define TAM_BUFFER 64
#define NTHREADS 4
#define MILLON 1000000

int buffer[TAM_BUFFER];
int p;
pthread_mutex_t m;

void *imprimir(void *p_numero) {
	
	int minumero = *((int *) p_numero);
   
    while(p < TAM_BUFFER)
    {
		pthread_mutex_lock(&m);
		
			if(p<TAM_BUFFER) {
				buffer[p] = minumero;
				printf("escribí un %i en la posición %i\n", minumero, p);
				p++;
			}
			
		pthread_mutex_unlock(&m);
		
		usleep(minumero*MILLON);
	}
	pthread_exit(NULL);
	
}                      

int main(int argc, char *argv[]) {
	int rc, j;
	int numero[NTHREADS];
	pthread_t tids[NTHREADS];
	pthread_attr_t attr;

	pthread_mutex_init(&m,NULL);
	
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	for (j=0; j<NTHREADS; j++) {
		numero[j] = j+1;
		rc = pthread_create(&tids[j], &attr, imprimir, &numero[j]);
		if (rc) {              
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}
	
	for (j=0; j<NTHREADS; j++) {
		/* Wait for the thread */
		rc = pthread_join(tids[j], NULL);
		if (rc) {
			printf("ERROR; return code from pthread_join() is %d\n", rc);
			exit(-1);
		}
	}

	for(p=0; p<TAM_BUFFER; p++) {
		printf("%d ", buffer[p]);
	}
	
	printf("\n");
	
	pthread_attr_destroy(&attr);
	
	
	pthread_exit(NULL);

}
