#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<unistd.h>
#include<sys/socket.h>
#include <netinet/in.h>
//Maximum Application Buffer
/**
    Max Amount of memory from network (Request Size)
*/
#define APP_MAX_BUFFER 1024
#define PORT 8080

int main(){
    //Client and server file descriptor
    int server_fd, client_fd;


    //Define socket address
    struct sockaddr_in server_address, client_address;
    int server_address_len=sizeof(server_address);
    int clinet_address_len=sizeof(client_address);

    //Data wil be moved to buffer here
    char buffer[APP_MAX_BUFFER]={0};

    server_fd=socket(AF_INET, SOCK_STREAM, 0);

    if(server_fd==-1){
        perror("Socket Failed");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family=AF_INET; //ipv4
    server_address.sin_port= htons(PORT) ;
    server_address.sin_addr.s_addr= INADDR_ANY; //Listening 0.0.0.0 inefraces

    if(bind(server_fd, (struct sockaddr *)&server_address, sizeof(server_address))<0){
        perror("Bind Failed");
        exit(EXIT_FAILURE);
    }

    //Creates the Queue
    //Listen for clients, with 10 backlog (10 connections in accept queue)
    if(listen(server_fd,10 )){
        perror("Listen Failed");
        exit(EXIT_FAILURE);
    }

    //We Loop Forever
    while(1){
        printf("Waiting for connection\n");
        //Accept A Client Connection
        client_fd=accept(server_fd, (struct sockaddr *)&client_address, (socklen_t *)&clinet_address_len);

        if(client_fd==-1){
            perror("Accept Failed");
            exit(EXIT_FAILURE);
        }

        read(client_fd, buffer, APP_MAX_BUFFER);
        printf("Huh? %s\n", buffer);

        char *http_response="HTTP/1.1 200 OK\nContent-Type: text.plain\nContent-Length: 13\n\nHello world!\n";

        write(client_fd, http_response, strlen(http_response));

        close(client_fd);
    }


    return 0;
}