all: client.c commit.c compression.c connection.c manifest.c update.c server.c WTFClient.c WTFServer.c
	mkdir c
	mkdir s
	gcc -g -O -o c/c WTFClient.c client.c commit.c compression.c connection.c manifest.c update.c -lcrypto -lm
	gcc -g -O -o s/s WTFServer.c server.c compression.c manifest.c -pthread -lm -lcrypto



clean:
	rm -r c
	rm -r s
	clear
	

cc: client.c commit.c compression.c connection.c manifest.c update.c WTFClient.c
	rm -r c
	mkdir c
	gcc -g -O -o c/c WTFClient.c client.c commit.c compression.c connection.c manifest.c update.c -lcrypto -lm
	
ss: server.c compression.c manifest.c WTFServer.c
	rm -r s
	mkdir s
	gcc -g -O -o s/s WTFServer.c server.c compression.c manifest.c -pthread -lm -lcrypto
