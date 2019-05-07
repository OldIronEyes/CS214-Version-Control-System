all: connection.c WTFClient.c manifest.c WTFServer.c update.c client.c compression.c
	gcc -g -O -o WTFClient WTFClient.c connection.c update.c manifest.c compression.c client.c -lcrypto -lm
	gcc -g -O -o WTFServer WTFServer.c -pthread

client: connection.c WTFClient.c manifest.c update.c client.c compression.c
	gcc -g -O -o WTFClient WTFClient.c connection.c update.c manifest.c client.c compression.c -lcrypto -lm

server: WTFserver.c WTFserver.h
	gcc -g -pthread -o WTFServer WTFserver.c

clean:
	rm WTF
	rm WTFServer
