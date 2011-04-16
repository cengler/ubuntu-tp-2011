#ifndef __SCHED_RR__
#define __SCHED_RR__

#include <vector>
#include <queue>
#include "basesched.h"

class SchedRR : public SchedBase {
	public:
		SchedRR(std::vector<int> argn);
		virtual void load(int pid);
		virtual void unblock(int pid);
		virtual int tick(const enum Motivo m);
	
	private:
		std::queue<int> q; /* cola de listos */
		int quantum;       /* tiempo maximo de ejecucion de un proceso */
		int quota;		   /* tiempo acumulado por el proceso actual */
};

#endif
