#include <vector>
#include <queue>
#include "sched_mfq.h"
#include "basesched.h"

#include <iostream>
#include <fstream>

using namespace std;

SchedMFQ::SchedMFQ(vector<int> argn) {
	quantums.swap(argn);
	qlength = quantums.size();
	qs = vector<queue<int> >(qlength);
	// CREO LAS qlength COLAS
	for(int i=0;i<qlength;i++) {
		queue<int> q;
		qs[i]=q;
	}
}

void SchedMFQ::load(int pid) {
	// LE SETEO PRIORIDAD MAXIMA Y LO ENCOLO A LA COLA DE MAXIMA PRIORIDAD
	p[pid] = MAX_PRIORITY;
	(qs[MAX_PRIORITY]).push(pid);
}

void SchedMFQ::unblock(int pid) {
	// SI TIENE LA PIORIDAD MAS ALTA LA DEJO EN ESA COLA
	if( p[pid] == MAX_PRIORITY )
		(qs[MAX_PRIORITY]).push(pid);
	else { // LE SUBIMOS LA PRIORIDAD
		p[pid] = p[pid]-1;
		(qs[p[pid]]).push(pid);
	}
}

int SchedMFQ::firstNotEmptyQueue() {
	for(int i=0;i<qlength;i++)
	{
		if(!qs[i].empty())
			return i;
	}
	return -1;
}

int SchedMFQ::tick(const enum Motivo m) {

	int q_index = firstNotEmptyQueue();
	
	// SI EL PROCESO TERMINO O SE BLOQUEO	
	if (m == EXIT) {
		if (q_index == -1) 
			return IDLE_TASK;
		else {
			quota = 0;
			int sig = qs[q_index].front(); qs[q_index].pop();
			return sig;
		}
	}
			
	if(m == BLOCK) {
		if (q_index == -1) 
			return IDLE_TASK;
		else {
			// TOMO EL SIG A EJECUTAR CON QUOTA EN 0
			quota = 0;
			int sig = qs[q_index].front(); qs[q_index].pop();
			return sig;
		}
	} 
	
	
	if(current_pid() == IDLE_TASK) {
		if (q_index == -1) 
			return IDLE_TASK;
		else {
			quota = 0;
			int sig = qs[q_index].front(); qs[q_index].pop();
			return sig;
		}
	}
	
	int currentPiority = p[current_pid()];
	int currentQuantum = quantums[currentPiority];
	//printf("currentPiority %d\n", currentPiority);
	//printf("currentQuantum %d\n", currentQuantum);
	// SE CUMPLIO LA CUOTA
	if(quota >= currentQuantum-1) {
		
		if (q_index == -1)
			return current_pid();
		
		// SI NO TIENE MINIMA PIORIDAD SE LA BAJO
		if( currentPiority != qlength-1 ) { 
			currentPiority++;
		}
		// AGREGO EL PROCESO EXPULSADO A LA COLA CON
		// LA PIORIDAD ACTUALIZADA
		p[current_pid()] = currentPiority;
		qs[currentPiority].push(current_pid());
		// LANZO EL SIG PROCESO CON QUOTA EN 0
		quota = 0;
		int sig = qs[q_index].front(); qs[q_index].pop();
		return sig;			
	}
	else { // NO CUMPLIO SU CUOTA NI SE BLOQUEO NI FINALIZO
		quota++;
		return current_pid();	
	}
}
