all: Connection_from_client.c WTFClient.c manifest.c WTFServer.c update.c client.c
	gcc -g -O -o WTFClient WTFClient.c Connection_from_client.c update.c manifest.c client.c -lcrypto -lm
	gcc -g -O -o WTFServer WTFServer.c -pthread

client: Connection_from_client.c Connection_from_client.h
	gcc -g -O -o WTFClient WTFClient.c Connection_from_client.c update.c manifest.c client.c -lcrypto -lm

server: WTFserver.c WTFserver.h
	gcc -g -pthread -o WTFServer WTFserver.c

clean:
	rm WTF
	rm WTFServer
