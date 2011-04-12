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
	rm -f *.o
	rm -f mt_server
	rm -f mt_client
	rm -f mt.h.gch
	rm -f *.zip

zip: clean all
	zip 02.zip mt_client.c mt.h mt_server.c Makefile
