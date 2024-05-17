#ifndef SERVER_HELPER_H
#define SERVER_HELPER_H
#include<string.h>
#include<stdio.h>
#include<stdlib.h>

struct HTTPServerResponse{
    int status_code;
    char *response_headers;
    char *response;
};

struct HTTPServerRequest{
    char* http_method;
    char *route;
    char *raw_buffer;
    //Can be extendend to include body etc
};

/**
Check Method
*/
void check(int status, char * error);

/**
Get Response of a request
*/
void get_response_of_request(struct HTTPServerResponse *response_obj, struct HTTPServerRequest *request_obj );
#endif