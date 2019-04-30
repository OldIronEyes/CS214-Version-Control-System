all: Connection_from_client.c Connection_from_client.h WTFserver.c WTFserver.h
	gcc -g -o WTF Connection_from_client.c
	gcc -g -pthread -o WTFServer WTFserver.c

clean:
	rm WTF
	rm WTFServer

