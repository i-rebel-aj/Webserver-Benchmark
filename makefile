clean:
	rm -rf ./out/*
default:
	gcc -c -g ./lib/server-helper.c -o ./out/server-helper.o
	ar rcs ./out/libserver-helper.a ./out/server-helper.o
	gcc -g -Wall -o ./out/single-thread-server -L./out/ single-thread-server.c -lserver-helper