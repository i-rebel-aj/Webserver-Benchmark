clean:
	rm -rf ./out/*
default:
	gcc -c ./lib/server-helper.c -o ./out/server-helper.o
	ar rcs ./out/libserver-helper.a ./out/server-helper.o
	gcc -g -Wall -o ./out/leaky-single-thread -L./out/ single-thread-webserver-leaky.c -lserver-helper