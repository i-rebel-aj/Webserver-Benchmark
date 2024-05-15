#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include "./server-helper.h"

char *get_file_name_from_route(char *route){
    if(strcmp(route, "/")==0){
        return "../html/index.html";
    }
    return "../html/404.html";
}

void get_html_page(char *route, char **html_page ){
    FILE *welcome_page=fopen(get_file_name_from_route(route), "r");
    if(welcome_page==NULL){
        printf("The corresponding HTML file for route %s not found:\n", route);
        char *empty_response="<html><body>Error opening corresponding file</body></html>";
        *html_page=malloc(strlen(empty_response)+1);
        strcpy(*html_page, empty_response);
        return;
    }
    char *file_content;
    //Calculate Size of file
    fseek(welcome_page, 0, SEEK_END);
    long unsigned int size=ftell(welcome_page);
    file_content=(char *)realloc(html_page,size+10);
    fseek(welcome_page, 0, SEEK_SET);
    fread(file_content, 1, size, welcome_page);
    file_content[size]='\0';
    *html_page=malloc(strlen(file_content)+1);
    strcpy(*html_page, file_content);
}
/**
Guard this method against poison messages, such as a HTTP request message which is illegal?
*/
void get_response_of_request(struct HTTPServerResponse *response_obj, char* request_buffer){
    char method[10];
    char *route=malloc(strlen(request_buffer)+1);
    sscanf(request_buffer, "%9s %s", method, route);
    printf("Method is %s", method);
    if(strcmp(method, "GET")!=0){
        char *response_headers="HTTP/1.1 404 Not Found\r\n";
        response_obj->response_headers=response_headers;
        return;
    }
    char *response_headers="HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    response_obj->response_headers=response_headers;
    response_obj->response=(char *)malloc(sizeof(char));
    get_html_page(route, &(response_obj->response));
    free(route);
    return;
}


void check(int ex, char *error){
    if(ex<0){
        perror(error);
        exit(EXIT_FAILURE);
    }
}