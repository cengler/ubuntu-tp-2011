#include "tasks.h"
#include <cstdlib>

using namespace std;

void TaskCPU(vector<int> params) { // params: n
	uso_CPU(params[0]); // Uso el CPU n milisegundos.
}

void TaskIO(vector<int> params) { // params: ms_cpu, ms_io,
	uso_CPU(params[0]); // Uso el CPU ms_cpu milisegundos.
	uso_IO(params[1]); // Uso IO ms_io milisegundos.
}

void TaskCon(vector<int> params) { // params: n
	int n = params[0];
	int bmin = params[1];
	int bmax = params[2];
	for( int i = 0; i < n ; i++ ) {
		int time = bmin;
		if(bmin != bmax)
			time = (rand()%(bmax-bmin))+bmin;
		uso_IO(time);
	}
}

void tasks_init(void) {
	/* Todos los tipos de tareas se deben registrar acá para poder ser usadas.
	 * El segundo parámetro indica la cantidad de parámetros que recibe la tarea
	 * como un vector de enteros, o -1 para una cantidad de parámetros variable. */
	register_task(TaskCPU, 1);
	register_task(TaskIO, 2);
	register_task(TaskCon, 3);

}
