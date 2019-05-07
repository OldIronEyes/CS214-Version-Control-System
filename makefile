all: test.c client.c commit.c compression.c connection.c manifest.c update.c
	gcc -g -O -o test test.c client.c commit.c compression.c connection.c manifest.c update.c -lcrypto -lm
	
clean:
	rm test
	clear
