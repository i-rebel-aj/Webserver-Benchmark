#ifndef SERVER_HELPER_H
#define SERVER_HELPER_H
#include<string.h>
#include<stdio.h>
#include<stdlib.h>

char *get_file_name_from_route(char *route);

/**
Based of the route provided, it returns a pointer to the HTML page 
*/
char* get_html_page(char *route );

#endif