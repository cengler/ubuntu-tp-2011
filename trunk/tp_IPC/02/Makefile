all: mt_server mt_client

mt_server : mt_server.o
	gcc -o mt_server mt_server.o

mt_client : mt_client.o
	gcc -o mt_client mt_client.o

mt_server.o : mt_server.c mt.h
	gcc -c mt_server.c mt.h

mt_client.o : mt_client.c mt.h
	gcc -c mt_client.c mt.h

clean:
	rm *.o
	rm mt_server
	rm mt_client
	rm mt.h.gch
