#include "mt.h"
int main(int argc, char* argv[])
{
	/* Validando argumentos*/
	if(argc != 2) {
		perror("Error missing operands\nPlease call mt_client <ip_server>\n");
		exit(1);
	}
	
	/* Crear socket sobre el que se escribe: dominio INET, protocolo UDP (DGRAM). */
	int sock;
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {
		perror("abriendo socket");
		exit(1);
	}
	
	/* Validacion y generacion de direccion del servidor*/
	struct sockaddr_in name;
	name.sin_family = AF_INET;
	name.sin_port = htons(PORT);
	if(inet_aton(argv[1], &name.sin_addr) == 0) {
		perror("Please insert a valid IP");
		exit(1);
	}

	/* Conectando con servidor */
	if (connect(sock, (void*) &name, sizeof(name))) {
		perror("Error on connection to server");
		exit(1);
	}

	/* Enviando mensaje */
	char text[MAX_MSG_LENGTH];
	for (;;) {
		fgets(text, MAX_MSG_LENGTH, stdin);

		/* Enviando mensaje */ 
		if(sendto(sock, text, sizeof(text), 0, (struct sockaddr *)&name, sizeof(name)) != sizeof(text)) {
			perror("Error sending message");
		}

		/* En caso de comando de salida salir */
		if (!strncmp(END_STRING, text, MAX_MSG_LENGTH)) {
			printf("Se envio mensaje de salida. \n");
			break;
		}
	}

	/* Cerrar socket de envio. */
	close(sock);
	return 0;
}
