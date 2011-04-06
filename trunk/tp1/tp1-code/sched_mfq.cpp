#include <vector>
#include <queue>
#include "sched_mfq.h"
#include "basesched.h"

using namespace std;

SchedMFQ::SchedMFQ(vector<int> argn) {
	quantums = argn;
	qlength = quantums.size();
}

void SchedMFQ::load(int pid) {
	p[pid] = qlength-1;
	(qs[qlength-1]).push(pid);
}

void SchedMFQ::unblock(int pid) {
	if( p[pid] == 0 )
		(qs[0]).push(pid);
	else {
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
	
	if (q_index != -1) 
			return IDLE_TASK;
	
	// SI EL PROCESO TERMINO O SE BLOQUEO	
	if (m == EXIT || m == BLOCK) {
		// TOMO EL SIG A EJECUTAR CON QUOTA EN 0
		quota = 0;
		int sig = qs[q_index].front(); qs[q_index].pop();
		return sig;
	} 
	
	int currentPiority = p[current_pid()];
	int currentQuantum = quantums[currentPiority];
	// SE CUMPLIO LA CUOTA
	if(quota >= currentQuantum) {
		
		// SI NO TIENE MINIMA PIORIDAD SE LA BAJO
		if( currentPiority != qlength-1 ) { 
			currentPiority--;
		}
		// AGREGO EL PROCESO EXPULSADO A LA COLA CON
		// LA PIORIDAD ACTUALIZADA
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
