default:
	gcc -g -Wall single-thread-webserver-leaky.c -o ./out/leaky-single
	gcc -g -Wall single-thread-webserver-non-leaky.c -o ./out/non-leaky-single
