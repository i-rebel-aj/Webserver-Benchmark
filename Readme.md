This is a learning repo

We'll be creating webservers in C and try to understand how things work under the hood.

Key things to mimic

- Network Bottlenecks
- Security
- OS Bottlenecks


In subsequent Parts I aim to make a webserver that serves a live video to client and bechmark it.


- 11 April 2024: Added a Leaky server with single threaded implementation. Lots to fix sigh

valgrind --tool=massif --massif-out-file=massif.out.2 ./leaky-server