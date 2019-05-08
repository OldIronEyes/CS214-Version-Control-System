all: client.c commit.c compression.c connection.c manifest.c update.c server.c WTFClient.c WTFServer.c
	mkdir Client
	mkdir Server
	gcc -g -O -o Client/WTFClient WTFClient.c client.c commit.c compression.c connection.c manifest.c update.c -lcrypto -lm
	gcc -g -O -o Server/WTFServer WTFServer.c WTF.h server.c compression.c manifest.c -pthread -lm -lcrypto

clean:
	rm -r Client
	rm -r Server
	rm WTFTest
	clear
