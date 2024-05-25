#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include "./server-helper.h"

/**
    This Returns Status code
*/
int get_file_path_for_route(char *route, char **file_path){
    if (getcwd(*file_path, 1024) == NULL) {
        printf("Current working directory: %s\n", *file_path);
    }
    if(strcmp(route, "/")==0){
        strcat(*file_path, "/html/index.html");
        return 200;
    }
    strcat(*file_path, "/html/404.html");
    return 404;
}

int get_html_page(char *route, char **html_page ){
    char *file_path=malloc(sizeof(char)*1024);
    int status_code=get_file_path_for_route(route, &file_path);
    FILE *welcome_page=fopen(file_path, "r");
    if(welcome_page==NULL){
        printf("The corresponding HTML file for route %s not found:\n", route);
        char *empty_response="<html><body>Error opening corresponding file, are paths fine?</body></html>";
        *html_page=malloc(strlen(empty_response)+1);
        strcpy(*html_page, empty_response);
        return 404;
    }
    char *file_content;
    //Calculate Size of file
    fseek(welcome_page, 0, SEEK_END);
    long unsigned int size=ftell(welcome_page);
    file_content=(char *)realloc(*html_page,size+10);
    fseek(welcome_page, 0, SEEK_SET);
    fread(file_content, 1, size, welcome_page);
    file_content[size]='\0';
    *html_page=malloc(strlen(file_content)+1);
    strcpy(*html_page, file_content);
    free(file_path);
    return status_code;
    
}
/**
Guard this method against poison messages, such as a HTTP request message which is illegal?
*/
void get_response_of_request(struct HTTPServerResponse *response_obj, struct HTTPServerRequest *request_obj){
    char method[10];
    char *route=malloc(strlen((request_obj->raw_buffer))+1);
    sscanf(request_obj->raw_buffer, "%9s %s", method, route);
    
    request_obj->http_method=method;
    request_obj->route=route;

    if(strcmp(method, "GET")!=0){
        char *response_headers="HTTP/1.1 404 Not Found\r\n";
        response_obj->response_headers=response_headers;
        (response_obj->status_code)=404;
        return;
    }
    response_obj->response=(char *)malloc(sizeof(char));
    int status_code=get_html_page(route, &(response_obj->response));
    response_obj->status_code=status_code;
    if(status_code==200){
        char *response_headers="HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
        response_obj->response_headers=response_headers;
        return;
    }
    char *response_headers="HTTP/1.1 404 Not Found\r\n";
    response_obj->response_headers=response_headers;
    return;
    
}


void check(int ex, char *error){
    if(ex<0){
        perror(error);
        exit(EXIT_FAILURE);
    }
}