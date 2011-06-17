#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include "mpi.h"

#define FALSE 0
#define TRUE  1

/* Distintos tipos de mensajes */
#define TAG_PEDIDO       		    10
#define TAG_OTORGADO       	   		20
#define TAG_LIBERO         	   		30
#define TAG_TERMINE           		40

/* Mensajes entre los servidores*/
#define TAG_PEDIDO_REMOTO			50
#define TAG_OTORGADO_REMOTO			60
#define TAG_TERMINE_REMOTO			70

/* Valores por defecto */
#define CANT_ITERACIONES             1
#define FACTOR_DELAY_COMPUTO    100000
#define FACTOR_DELAY_CRITICA        10

/* Variables globales */
int cant_ranks, mi_rank, mi_rol, mi_nro;

void debug(const char *s)
{
#ifndef NDEBUG
    printf("R%-3d %c%-3d %-50s\n", mi_rank, mi_rol?'C':'S', mi_nro, s);
#endif
}

void servidor(int mi_cliente)
{
	/* Info de status */
    MPI_Status status;
    int origen, tag;
    
    /* Numeros de secuencia */
    int nro_seq_recibido;
    int mayor_nro_seq_recibido 		= -1;
    int nro_seq 					= 0;
    
    /* Pedido de mutex local */
    int hay_pedido_local = FALSE;
    
    /* Cantidad de permisos remotos (TAG_OTORGADO_REMOTO) 
     * que necesito para entrar */
    int otorgados_remotos_faltantes = -1;
    
    /* Cantidad de servidores */
    int cant_servidores = cant_ranks/2;
    
    /* Cola de pedidos de mutex demorados 
     * (TRUE indica que ese servidor pidio el mutex) */
    int cola_de_pedidos[cant_servidores];
    
    /* Cantidad de servidores que todavia no terminaron */
    int servidores_terminados_faltantes = cant_servidores;
    
    /* inicializo la cola de pedidos en "vacio" */
    int i;
    for(i=0;i<cant_servidores;i++) {
		cola_de_pedidos[i] = FALSE;
	}
    
    while(TRUE) {
        
        MPI_Recv(&nro_seq_recibido, 1, MPI_INT, MPI_ANY_SOURCE, 
					MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        origen = status.MPI_SOURCE;
        tag = status.MPI_TAG;
        
        /* Actualizo el mayor numero de secuencia obtenido */
        if(mayor_nro_seq_recibido < nro_seq_recibido)
			mayor_nro_seq_recibido = nro_seq_recibido;
			
		
        if (tag == TAG_TERMINE) 
        {
			debug("TAG_TERMINE");
			
            int serv;
            for(serv = 0; serv<cant_servidores; serv++) {
				if (serv != mi_rank/2)
					MPI_Send(NULL, 0, MPI_INT,serv*2, TAG_TERMINE_REMOTO, MPI_COMM_WORLD);
			}
			servidores_terminados_faltantes--;
			if(servidores_terminados_faltantes == 0)
				break;
        }
        
        if( tag == TAG_TERMINE_REMOTO )
        {
			debug("TAG_TERMINE_REMOTO");
			assert(origen != mi_cliente);
			servidores_terminados_faltantes--;
			if(servidores_terminados_faltantes == 0)
				break;
		}

        if (tag == TAG_PEDIDO) {
			debug("TAG_PEDIDO");
            assert(origen == mi_cliente);
            assert(hay_pedido_local == FALSE);
            
            hay_pedido_local = TRUE;
            
            /* Si hay un solo servidor no tengo que pedir
             * permiso a nadie */
            if(cant_servidores == 1) {
				MPI_Send(NULL, 0, MPI_INT, mi_cliente, TAG_OTORGADO, MPI_COMM_WORLD);
			}
            else {		
				/* Actualizo el numero de secuencia */
				if(mayor_nro_seq_recibido > nro_seq)
					nro_seq = mayor_nro_seq_recibido;
				nro_seq++;
			
				/* Le pido permisos a todos los servers (a mi mismo no) */
				otorgados_remotos_faltantes = cant_servidores-1;
				int serv;
				for(serv = 0; serv<cant_servidores; serv++) {
					if (serv != mi_rank/2) {
						MPI_Send(&nro_seq, 1, MPI_INT, serv*2, TAG_PEDIDO_REMOTO, MPI_COMM_WORLD);
					}
				}
			}
        }
        
        if (tag == TAG_LIBERO) {
			debug("TAG_LIBERO");
            assert(origen == mi_cliente);
            assert(hay_pedido_local == TRUE);
            hay_pedido_local = FALSE;
            
            /* Libero todos los pedidos demorados */
            int i;
			for(i=0;i<cant_servidores;i++)
			{
				if(cola_de_pedidos[i] == TRUE)
				{
					assert(i != (mi_rank/2)); 
					MPI_Send(NULL, 0, MPI_INT, i*2, TAG_OTORGADO_REMOTO, MPI_COMM_WORLD);
					cola_de_pedidos[i] = FALSE;
				}
			}
        }
        
        if (tag == TAG_PEDIDO_REMOTO) {
			debug("TAG_PEDIDO_REMOTO");
			assert(origen != mi_cliente);
			assert(origen != mi_rank);
			
			/* si yo no lo tengo 
			 * y lo pidieron antes que yo
			 * o lo pedimos al mismo tiempo y tiene menor rank	
			 * ENTREGO EL MUTEX */
			if(!hay_pedido_local || nro_seq_recibido < nro_seq || 
				( (nro_seq_recibido == nro_seq) && mi_rank > origen))
				MPI_Send(NULL, 0, MPI_INT, origen, TAG_OTORGADO_REMOTO, MPI_COMM_WORLD);
			else {
				/* Encolo el pedido para liberarlo cuando deje de 
				 * usar el mutex */
				cola_de_pedidos[origen/2] = TRUE;
			}
		}
		
		if (tag == TAG_OTORGADO_REMOTO) {
			debug("TAG_OTORGADO_REMOTO");
			assert(origen != mi_cliente);
			assert(origen != mi_rank);

			/* Si todos me los otorgaron le doy permiso a entrar en 
			 * el la zona critica */
			otorgados_remotos_faltantes--;
			if (otorgados_remotos_faltantes == 0)
				MPI_Send(NULL, 0, MPI_INT, mi_cliente, TAG_OTORGADO, MPI_COMM_WORLD);
		}
    }
}

void cliente(int mi_serv, int cant_iters, int delay_comp, int delay_crit)
{
    long mi_delay_comp = delay_comp * (mi_nro + 1);
    long mi_delay_crit = delay_crit * (mi_nro + 1);
    char mi_char = 'a' + mi_nro;
    MPI_Status status; int i;

    while(cant_iters-- > 0) {
		
        usleep(mi_delay_comp);

        debug("Enviando pedido a mi servidor");
        MPI_Send(NULL, 0, MPI_INT, mi_serv, TAG_PEDIDO, MPI_COMM_WORLD);

        debug("Esperando respuesta de mi servidor");
        MPI_Recv(NULL, 0, MPI_INT, mi_serv, TAG_OTORGADO, MPI_COMM_WORLD, &status);

        debug("Entrando en sección crítica");
        for(i = 0; i < 40; ++i) {
            usleep(mi_delay_crit);
            fprintf(stderr, "%c", mi_char);
        } fprintf(stderr, "\n");
        
        usleep(100000); 
        
        debug("Saliendo de sección crítica");
        MPI_Send(NULL, 0, MPI_INT, mi_serv, TAG_LIBERO, MPI_COMM_WORLD);
    }

    debug("Enviando mensaje final a mi servidor");
    MPI_Ssend(NULL, 0, MPI_INT, mi_serv, TAG_TERMINE, MPI_COMM_WORLD);
}


void leer_opciones(int argc, char **argv, int *iters, int *dcomp, int *dcrit)
{
    char *ep;
    if(argc > 1) { *iters = strtol(argv[1], &ep, 10); assert(!*ep); }
    if(argc > 2) { *dcomp = strtol(argv[2], &ep, 10); assert(!*ep); }
    if(argc > 3) { *dcrit = strtol(argv[3], &ep, 10); assert(!*ep); }
}


int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &mi_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &cant_ranks);
    assert(cant_ranks % 2 == 0);
    mi_rol = mi_rank % 2;
    mi_nro = mi_rank / 2;

    if (mi_rol == 0)
        servidor(mi_rank + 1);

    else if (mi_rol == 1) {
        int cant_iters = CANT_ITERACIONES;
        int delay_comp = FACTOR_DELAY_COMPUTO;
        int delay_crit = FACTOR_DELAY_CRITICA;
        leer_opciones(argc, argv, &cant_iters, &delay_comp, &delay_crit);
        cliente(mi_rank - 1, cant_iters, delay_comp, delay_crit);
    }

    debug("Finalizando");

    MPI_Finalize();
    return 0;
}

