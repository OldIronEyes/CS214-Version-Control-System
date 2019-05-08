all: client.c commit.c compression.c connection.c manifest.c update.c server.c WTFClient.c WTFServer.c
	mkdir client
	mkdir server
	gcc -g -O -o client/WTFClient WTFClient.c client.c commit.c compression.c connection.c manifest.c update.c -lcrypto -lm
	gcc -g -O -o server/WTFServer WTFServer.c WTF.h server.c compression.c manifest.c -pthread -lm -lcrypto

client: client.c commit.c compression.c connection.c manifest.c update.c WTFClient.c
	mkdir client
	gcc -g -O -o client/WTFClient WTFClient.c client.c commit.c compression.c connection.c manifest.c update.c -lcrypto -lm


server: WTFServer.c server.c compression.c manifest.c
	mkdir server
	gcc -g -O -o server/WTFServer WTFServer.c server.c compression.c manifest.c -pthread -lm -lcrypto


clean:
	rm -r client
	rm -r server
	clear
