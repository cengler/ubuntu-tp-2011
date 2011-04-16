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
		/* Metodo que retorna el numero de la primer cola no vacia.
		 * en caso que que sean todas vacias retorna -1 */
		virtual int firstNotEmptyQueue(); 
		
		int qlength;						/* cantidad de colas RR */
		std::vector<int> quantums;			/* quantum de cada cola */
		std::vector<std::queue<int> > qs;	/* lista de colas RR */
		std::map<int,int> p;				/* pioridad de cada proceso */
		int quota;							/* ciclos consumidos por el proceso actual */

};

#endif
