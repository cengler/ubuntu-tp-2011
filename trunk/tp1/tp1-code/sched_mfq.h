#ifndef __SCHED_MFQ__
#define __SCHED_MFQ__

#include <vector>
#include <queue>
#include "basesched.h"

class SchedMFQ : public SchedBase {
	public:
		SchedMFQ(std::vector<int> argn);
		virtual void load(int pid);
		virtual void unblock(int pid);
		virtual int tick(const enum Motivo m);
	
	private:
		/* Completar */
};

#endif
