#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<signal.h>
#include<pthread.h>
#include <netinet/in.h>
#include "server-helper.h"
//Maximum Application Buffer
/**
    Max Amount of memory from network (Request Size)
*/
#define APP_MAX_BUFFER 1024
#define MAX_BACKLOG_QUEUE_SIZE 10


void send_response(int _server_fd){
    struct sockaddr_in client_address;
    int clinet_address_len=sizeof(client_address);
    int client_fd=accept(_server_fd, (struct sockaddr *)&client_address, (socklen_t *)&clinet_address_len);
    
    if(client_fd==-1){
        perror("Accept Failed");
        exit(EXIT_FAILURE);
    }
    char buffer[APP_MAX_BUFFER]={0};
    if(read(client_fd, buffer, APP_MAX_BUFFER)==-1){
        perror("Request can not read to buffer\n");
    }
    char request_path[1024];
    
    char * html_page=get_html_page(request_path);
    const char *response_header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    write(client_fd, response_header, strlen(response_header));
    write(client_fd, html_page, strlen(html_page));
    close(client_fd);
}

int main(int argc, char **argv){

    uint PORT;
    if(argc < 2){
        printf("Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    if(sscanf(argv[1], "%u", &PORT) != 1){
        printf("Invalid port number\n");
        exit(EXIT_FAILURE);
    }
    //Define socket address
    struct sockaddr_in server_address;

    int server_fd=socket(AF_INET, SOCK_STREAM, 0);

    if(server_fd==-1){
        perror("Socket Failed");
        exit(EXIT_FAILURE);
    }
    /**
        Make Socket Reusable Even if server is closed
    */
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
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
    if(listen(server_fd,MAX_BACKLOG_QUEUE_SIZE )){
        perror("Listen Failed");
        exit(EXIT_FAILURE);
    }
    printf("Server %s Running on PORT %d\nPress Ctrl+C to exit...\n", argv[2], PORT);
   
    while(1){
        printf("Waiting for connection\n");
        send_response(server_fd);
    }

    return 0;
}