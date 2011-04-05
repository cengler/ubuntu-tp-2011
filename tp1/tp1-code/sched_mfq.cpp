#include <vector>
#include <queue>
#include "sched_mfq.h"
#include "basesched.h"

using namespace std;

SchedMFQ::SchedMFQ(vector<int> argn) {
	// MFQ recibe los n quantums por parámetro
	/* Completar */
}

void SchedMFQ::load(int pid) {
	/* Completar */
}

void SchedMFQ::unblock(int pid) {
	/* Completar */
}

int SchedMFQ::tick(const enum Motivo m) {
	/* Completar */
	return IDLE_TASK;
}
