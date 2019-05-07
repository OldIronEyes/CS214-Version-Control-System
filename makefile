all: client.c commit.c compression.c connection.c manifest.c update.c server.c WTFClient.c WTFServer.c
	gcc -g -O -o WTFClient WTFClient.c client.c commit.c compression.c connection.c manifest.c update.c -lcrypto -lm
	gcc -g -O -o WTFServer WTFServer.c server.c -pthread

clean:
	rm WTFClient
	rm WTFServer
	clear
