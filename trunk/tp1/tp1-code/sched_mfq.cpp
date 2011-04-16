#include <vector>
#include <queue>
#include "sched_mfq.h"
#include "basesched.h"

#include <iostream>
#include <fstream>

using namespace std;

SchedMFQ::SchedMFQ(vector<int> argn) {
	/* salvamos los quantums en una variable de clase */
	quantums.swap(argn);
	/* cantidad de colas */
	qlength = quantums.size();
	/* creacion de las qlength colas */
	qs = vector<queue<int> >(qlength);
	for(int i=0;i<qlength;i++) {
		queue<int> q;
		qs[i]=q;
	}
}

void SchedMFQ::load(int pid) {
	/* cuando un proceso llega se le asigna la maxima pioridad */
	p[pid] = MAX_PRIORITY;
	/* se lo encola en la cola correspondiente */
	(qs[MAX_PRIORITY]).push(pid);
}

void SchedMFQ::unblock(int pid) {
	/* Si no tiene pioridad maxima, aumento su pioridad 
	 * (por haber sido bloqueado por e/s) */
	if( p[pid] != MAX_PRIORITY )
		p[pid] = p[pid]-1;
	/* se lo encola en la cola correspondiente */
	(qs[p[pid]]).push(pid);
}

int SchedMFQ::tick(const enum Motivo m) {
	/* Cola de la cual obtendre el sig proceso a ejecutar */
	int q_index = firstNotEmptyQueue();
	
	/* Si se termino o se bloqueo el actual */
	if (m == EXIT || m == BLOCK) {
		if (q_index == -1) /* Si no hay nada mas se ejecuta IDLE */
			return IDLE_TASK;
		else { /* si hay algo mas se ejecuta el siguiente */
			quota = 0;
			int sig = qs[q_index].front(); qs[q_index].pop();
			return sig;
		}
	} 
	
	/* Si se recibe TICK y no estaba ejecutando nada */
	if(current_pid() == IDLE_TASK) {
		if (q_index == -1) /* No hay nada mas para ejecutar retorno IDLE */
			return IDLE_TASK;
		else { /* Si hay mas procesos ejecuto el siguiente de la cola */
			quota = 0;
			int sig = qs[q_index].front(); qs[q_index].pop();
			return sig;
		}
	}
	
	/* Si se recibe TICK y se estaba ejecutando algo */
	
	int currentPiority = p[current_pid()]; /* pioridad del proceso corriente */
	int currentQuantum = quantums[currentPiority]; /* quantum de la cola corriente */
	
	/* si se cumplio la quota */
	if(quota >= currentQuantum-1) {
		/* No hay nada mas prara ejecutar continuo ejecutando el mismo proceso */
		if (q_index == -1) 
			return current_pid();
		/* se le disminuye su pioridad */
		if( currentPiority != qlength-1 ) { 
			currentPiority++;
		}
		/* Se agrega el proceso expulsado a la cola de pioridad actualizada */
		p[current_pid()] = currentPiority;
		qs[currentPiority].push(current_pid());
		/* Se lanza el siguiente proceso */
		quota = 0;
		int sig = qs[q_index].front(); qs[q_index].pop();
		return sig;			
	}
	else { /* Si no se cumplio la quota */
		quota++;
		return current_pid();	
	}
}

int SchedMFQ::firstNotEmptyQueue() {
	/* Metodo que retorna el numero de la primer cola no vacia.
	 * en caso que que sean todas vacias retorna -1 */
	for(int i=0;i<qlength;i++)
	{
		if(!qs[i].empty())
			return i;
	}
	return -1;
}
