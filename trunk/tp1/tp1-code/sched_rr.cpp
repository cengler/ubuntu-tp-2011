#include <vector>
#include <queue>
#include "sched_rr.h"
#include "basesched.h"

using namespace std;

SchedRR::SchedRR(vector<int> argn) {
	/* Me guardo el quantum */
	quantum = argn[0]; 
}

void SchedRR::load(int pid) {
	/* Lo encolo a la cola de listos */
	q.push(pid);
}

void SchedRR::unblock(int pid) {
	/* Lo encolo a la cola de listos */
	q.push(pid); 
}

int SchedRR::tick(const enum Motivo m) {
	
	/* Si el motivo es EXIT o BLOCK */
	if (m == EXIT || m == BLOCK) {
		
		if (q.empty()) return IDLE_TASK; /* No hay nada mas para ejecutar retorno IDLE */
		else { /* Si hay mas procesos ejecuto el siguiente de la cola */
			quota = 0;
			int sig = q.front(); q.pop();
			return sig;
		}
	} 
	
	/* Si se recibe TICK y no estaba ejecutando nada */
	if (current_pid() == IDLE_TASK) {
		if (q.empty()) return IDLE_TASK; /* No hay nada mas para ejecutar retorno IDLE */
		else { /* Si hay mas procesos ejecuto el siguiente de la cola */
			quota = 0;
			int sig = q.front(); q.pop();
			return sig;
		}
	} 
	
	/* Si se recibe TICK y se estaba ejecutando algo */
	if (q.empty()) { /* No hay ningun otro proceso, retorno el mismo */
		return current_pid();
	} else if(quota >= quantum-1) { /* Si supero su quantum lo desalojo y ejecuto el sig */
		quota = 0;
		int c_pid = current_pid();
		q.push(c_pid);
		int sig = q.front(); q.pop();
		return sig;
	} else { /* Si no alcanzo su quantum, incremento el acumulador de quota */
		quota++;
		return current_pid();
	}
}
