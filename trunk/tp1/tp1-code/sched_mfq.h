#ifndef __SCHED_MFQ__
#define __SCHED_MFQ__
#define MAX_PRIORITY 0

#include <vector>
#include <queue>
#include <map>
#include "basesched.h"

class SchedMFQ : public SchedBase {
	public:
		SchedMFQ(std::vector<int> argn);
		virtual void load(int pid);
		virtual void unblock(int pid);
		virtual int tick(const enum Motivo m);
	
	private:
		virtual int firstNotEmptyQueue();
		int qlength;
		std::vector<int> quantums;
		std::vector<std::queue<int> > qs;
		std::map<int,int> p;
		int quota;

};

#endif
