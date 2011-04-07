#include <vector>
#include <queue>
#include "sched_rr.h"
#include "basesched.h"

using namespace std;

SchedRR::SchedRR(vector<int> argn) {
	quantum = argn[0]; // ME GUARDO AL QUANTUM
}

void SchedRR::load(int pid) {
	q.push(pid); // CARGO EL SIGUIENTE A EJECUTAR
}

void SchedRR::unblock(int pid) {
	q.push(pid);  // LA AGREGO A LA COLA DE LISTOS
}

int SchedRR::tick(const enum Motivo m) {
	
	// SI TERMINO EJECUTO LA SIGUIENTE POR TODO UN QUANTUM
	if (m == EXIT) {
		if (q.empty()) return IDLE_TASK;
		else {
			quota = 0;
			int sig = q.front(); q.pop();
			return sig;
		}
	} 
	
	// SI SE BLOQUEO
	if (m == BLOCK) {
		if (q.empty()) return IDLE_TASK;
		else {
			quota = 0;
			int sig = q.front(); q.pop();
			return sig;
		}
	} 
	
	// SI SE CUMPLIO UN CICLO NORMAL
	if (current_pid() == IDLE_TASK) {
		if (q.empty())
			return IDLE_TASK;
		else {
			quota = 0;
			int sig = q.front(); q.pop();
			return sig;
		}
	} else { //Hay algo ejecutandose.
		if (q.empty()) { // Si no hay nada mas para ejecutarse, sigo.
			quota = 0;
			return current_pid();
		} else if(quota >= quantum-1) {
			quota = 0;
			int c_pid = current_pid();
			q.push(c_pid);
			int sig = q.front(); q.pop();
			return sig;
		} else {
			quota++;
			return current_pid();
		}
	}
}
