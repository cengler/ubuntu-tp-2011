#include "mt.h"
int main(int argc, char* argv[])
{
	/* Validando argumentos*/
	if(argc != 2) {
		perror("missing operands\nPlease call mt_client <ip_server>\n");
		exit(1);
	}

	/* Crear socket sobre el que se escribe: dominio INET, protocolo TCP (STREAM). */
	int sock;	
	sock = socket(AF_INET, SOCK_STREAM, 0);
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
		
		memset(text,0,MAX_MSG_LENGTH);
		fgets(text, MAX_MSG_LENGTH, stdin);
		
		if ( send(sock, text, strlen(text)+1, 0) < 0 ) {
			perror("ERROR escribiendo en el  socket");
			break;
		}
		
		if (!strncmp(text, END_STRING, MAX_MSG_LENGTH)) {
			printf("Desconectando del servidor\n");
			break;
		}	
		
		int readed = read(sock, text, MAX_MSG_LENGTH);
		if( readed < 0 ) {
			perror("ERROR leyendo el socket");
		}
		
		write(STD_OUT, text, readed);
	}

	close(sock);
	return 0;
}
