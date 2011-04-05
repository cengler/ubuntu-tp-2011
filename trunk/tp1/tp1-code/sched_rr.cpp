#include <vector>
#include <queue>
#include "sched_rr.h"
#include "basesched.h"

using namespace std;

SchedRR::SchedRR(vector<int> argn) {
	// Round robin recibe el quantum por parámetro
	/* Completar */
}

void SchedRR::load(int pid) {
	/* Completar */
}

void SchedRR::unblock(int pid) {
	/* Completar */
}

int SchedRR::tick(const enum Motivo m) {
	/* Completar */
	return IDLE_TASK;
}
