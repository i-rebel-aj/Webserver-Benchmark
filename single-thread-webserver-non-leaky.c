#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<signal.h>
#include<pthread.h>
#include <netinet/in.h>
//Maximum Application Buffer
/**
    Max Amount of memory from network (Request Size)
*/
#define APP_MAX_BUFFER 1024
#define PORT 3001
#define MAX_BACKLOG_QUEUE_SIZE 10
#define SUCCESS_RESPONSE "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\0"


char* get_html_page(){
    FILE *welcome_page=fopen("index.html", "r");
    char *file_content;
    //Calculate Size of file
    fseek(welcome_page, 0, SEEK_END);
    long unsigned int size=ftell(welcome_page);
    file_content=(char *)malloc(size+10);
    fseek(welcome_page, 0, SEEK_SET);
    fread(file_content, 1, size, welcome_page);
    file_content[size]='\0';
    fclose(welcome_page);
    return file_content;
}

void send_response(int _server_fd){
    struct sockaddr_in client_address;
    int clinet_address_len=sizeof(client_address);
    //printf("Initial Client Address len is %d %d\n", clinet_address_len, client_address.sin_addr.s_addr);
    int client_fd=accept(_server_fd, (struct sockaddr *)&client_address, (socklen_t *)&clinet_address_len);
    //printf("Client Address Post Connection is %d %d\n", clinet_address_len, client_address.sin_addr.s_addr);
    if(client_fd==-1){
        perror("Accept Failed");
        exit(EXIT_FAILURE);
    }
    char buffer[APP_MAX_BUFFER]={0};
    if(read(client_fd, buffer, APP_MAX_BUFFER)==-1){
        perror("Request can not read to buffer\n");
    }
    char * html_page=get_html_page();
    const char *response_header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    write(client_fd, response_header, strlen(response_header));
    write(client_fd, html_page, strlen(html_page));
    free(html_page);
    close(client_fd);
}

int main(){
    
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
    printf("Server running at PORT %d, press Ctrl+C to exit...\n", PORT);
   
    while(1){
        printf("Waiting for connection\n");
        send_response(server_fd);
    }

    return 0;
}