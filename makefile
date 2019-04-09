all: WTFClient.c WTFClient.h WTFServer.c WTFServer.h
	gcc -O -g -o WTF WTFClient.c WTFClient.h
	gcc -O -g -o WTFServer WTFServer.c WTFServer.h

test: WTFtest.c WTFtest.h
	gcc -O -g -o WTFtest WTFtest.c WTFtest.h

clean:
	rm WTF
	rm WTFserver
	rm WTFtest
