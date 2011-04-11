#include "tasks.h"
#include <cstdlib>

#include <iostream>
#include <fstream>

using namespace std;

void TaskCPU(vector<int> params) { // params: n
	uso_CPU(params[0]); // Uso el CPU n milisegundos.
}

void TaskIO(vector<int> params) { // params: ms_cpu, ms_io,
	uso_CPU(params[0]); // Uso el CPU ms_cpu milisegundos.
	uso_IO(params[1]); // Uso IO ms_io milisegundos.
}

void TaskIOMultiple(vector<int> params) { // params: n
	for( int i = 0; i < params[0] ; i++ ) {
		uso_IO(1); // Uso IO ms_io milisegundos.
	}
}

void TaskCon(vector<int> params) { // params: 3
	int n = params[0];
	int bmin = params[1];
	int bmax = params[2];
	
	// INICIALIZAMOS SEMILLA DE RAND()
	srand ( time(NULL) );
	
	for( int i = 0; i < n ; i++ ) {
		int time = bmin;
		if(bmin != bmax)
			time = (rand()%(bmax-bmin+1))+bmin;
		uso_IO(time);
	}
}

void TaskCPUIOCPU(vector<int> params) { // params: ms_cpu1, ms_io, ms_cpu2
	uso_CPU(params[0]); // Uso el CPU ms_cpu1 milisegundos.
	uso_IO(params[1]); // Uso IO ms_io milisegundos.
	uso_CPU(params[2]); // Uso el CPU ms_cpu2 milisegundos.
}

void TaskLongCPUMultipleIO(vector<int> params) { // params: n
	uso_CPU(params[0]); // Uso el CPU ms_cpu milisegundos.
	int n = params[1];
	int bmin = params[2];
	int bmax = params[3];
	
	// INICIALIZAMOS SEMILLA DE RAND()
	srand ( time(NULL) );
	
	for( int i = 0; i < n ; i++ ) {
		int time = bmin;
		if(bmin != bmax)
			time = (rand()%(bmax-bmin))+bmin;
		uso_IO(time);
	}
}

void TaskMultipleIOLongCPU(vector<int> params) { // params: n
	int n = params[1];
	int bmin = params[2];
	int bmax = params[3];
	
	// INICIALIZAMOS SEMILLA DE RAND()
	srand ( time(NULL) );
	
	for( int i = 0; i < n ; i++ ) {
		int time = bmin;
		if(bmin != bmax)
			time = (rand()%(bmax-bmin))+bmin;
		uso_IO(time);
	}
	uso_CPU(params[0]); // Uso el CPU ms_cpu milisegundos.
}

void TaskBatch(vector<int> params) {
	int tot = params[0];
	int blocksC = params[1];
	
	// INICIALIZAMOS SEMILLA DE RAND()
	srand ( time(NULL) );
	
	vector<bool> blocks = vector<bool>(tot-1);
	for(int i=0;i<blocksC;i++) {
		int blockT = rand()%(blocks.size()-1);
		if( !blocks[blockT] )
			blocks[blockT] = true;
		else
			i--;
	}
	for(int i=0;i<(int)blocks.size();i++) {
		if( blocks[i] )
			uso_IO(1);
		else
			uso_CPU(1);
	}
}

void tasks_init(void) {
	/* Todos los tipos de tareas se deben registrar acá para poder ser usadas.
	 * El segundo parámetro indica la cantidad de parámetros que recibe la tarea
	 * como un vector de enteros, o -1 para una cantidad de parámetros variable. */
	register_task(TaskCPU, 1);
	register_task(TaskIO, 2);
	register_task(TaskCon, 3);
	register_task(TaskLongCPUMultipleIO, 4);
	register_task(TaskMultipleIOLongCPU, 4);
	register_task(TaskBatch, 2);
	register_task(TaskCPUIOCPU, 3);
	register_task(TaskIOMultiple, 1);
}
