#include "mt.h"
int main()
{
	/* Crear socket sobre el que vamos a aceptar conexiones: dominio INET, protocolo TCP (STREAM). */
	int server_socket;
	if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Error abriendo socket");
		exit(1);
	}
	
	/* Crear la direccion donde abriremos el socket */
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(PORT);
	
	/* Conectamos el socket a la direccion que creamos */
	if (bind(server_socket, (void*) &server_address, sizeof(server_address))) {
		perror("binding datagram socket");
		exit(1);
	}

	/* Preparamos el socket para recibir conexiones */
	if (listen(server_socket, 5)) {
        	perror("Error escuchando");
        	exit(1);
    }

	/* Preparamos los punteros para recibir la direccion del cliente y aceptamos el primer cliente */
	int client_socket;
	int client_address;
	int client_address_len = sizeof(client_address);
	if ( (client_socket = accept(server_socket, (struct sockaddr *) &client_address, &client_address_len)) < 0)
	{
		perror("ERROR aceptando conexion");
		exit(1);
	}
	 
	/* Vamos a aceptar mensajes hasta que el cliente finalice la conexion */
	char recived_buffer[MAX_MSG_LENGTH]; 
	memset(recived_buffer,0,MAX_MSG_LENGTH);
	for (;;) {

		/*Recibimos del cliente*/
		if ( recv(client_socket, recived_buffer, MAX_MSG_LENGTH, 0) < 0 ) {
			perror("ERROR leyendo el socket");
			break;
		}
		
		if (strncmp(recived_buffer, END_STRING, MAX_MSG_LENGTH) == 0) {
			printf("El cliente nos envio un chau\n");
			break;
		} else {
			printf("Recibimos el comando %s", recived_buffer);
		}

		/* Salvamos los File Descriptors de la salida standard y salida de error */
		int std_out = dup(STD_OUT);
		int std_err = dup(STD_ERR);

		/* Redirigimos la salida estandar y la salida de error al socket cliente */
		dup2(client_socket, STD_OUT);
		dup2(client_socket, STD_ERR);
		
		/* Ejecutamos el comando recibido */		
		system(recived_buffer);
		fflush(stdout);
		
		/* Restauramos las salidas estandar */
		dup2(std_out, STD_OUT);
		dup2(std_err, STD_ERR);

		/* Cerramos los File Descriptors */
		close(std_out);
		close(std_err);		
	}
	
	/* Cerramos los sockets */
	close(client_socket);
	close(server_socket);
	
	return 0;
}

