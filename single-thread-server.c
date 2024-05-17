#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<signal.h>
#include<pthread.h>
#include <netinet/in.h>
#include "./lib/server-helper.h"
//Maximum Application Buffer
/**
    Max Amount of memory from network (Request Size)
*/
#define APP_MAX_BUFFER 1024
#define MAX_BACKLOG_QUEUE_SIZE 10

/**
The Following Method, accepts the request comming on webserver
and sends an HTTP Response
*/
void handle_request_on_server(int _server_fd){
    struct sockaddr_in client_address;
    int clinet_address_len=sizeof(client_address), client_fd;
    
    check(client_fd=accept(_server_fd, (struct sockaddr *)&client_address, (socklen_t *)&clinet_address_len), "Accept Failed, can not get a client File descriptor");
    
    char request_buffer[APP_MAX_BUFFER]={0};
    if(read(client_fd, request_buffer, APP_MAX_BUFFER)==-1){
        perror("Request can not read to max buffer\n");
    }
    // printf("Read Buffer is %s\n", request_buffer);
    
    struct HTTPServerResponse *response=malloc(sizeof( struct HTTPServerResponse));
    struct HTTPServerRequest *request=malloc(sizeof(struct HTTPServerRequest));
    request->raw_buffer=request_buffer;
    
    get_response_of_request(response, request);

    printf("\033[35m %s %s --- %d\n", request->http_method, request->route, response->status_code);

    write(client_fd, (*response).response_headers, strlen((*response).response_headers));
    write(client_fd, (*response).response, strlen((*response).response));
    
    free(response);
    free(request);
    close(client_fd);
}

int main(int argc, char **argv){

    uint PORT;
    int server_fd;
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

    check(server_fd=socket(AF_INET, SOCK_STREAM, 0), "Socket Failed"); 

    if(server_fd==-1){
        perror("Socket Failed");
        exit(EXIT_FAILURE);
    }
    /**
        Make Socket Reusable Even if server is closed
    */
    int opt = 1;
    check(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)), "Can not set socket as reusable"); 
    
    server_address.sin_family=AF_INET; //ipv4
    server_address.sin_port= htons(PORT) ;
    server_address.sin_addr.s_addr= INADDR_ANY; //Listening 0.0.0.0 inefraces

    check(bind(server_fd, (struct sockaddr *)&server_address, sizeof(server_address)), "Can not bind address to server file descriptor");
    

    check(listen(server_fd,MAX_BACKLOG_QUEUE_SIZE ), "Listen Failed on server file descriptor");
    
    printf("Server %s Running on PORT %d\nPress Ctrl+C to exit...\n", argv[2], PORT);

    while(1){
        
        handle_request_on_server(server_fd);
    }

    return 0;
}