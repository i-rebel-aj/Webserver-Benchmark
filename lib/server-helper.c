#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include "../server-helper.h"

char *get_file_name_from_route(char *route){
    if(strcmp(route, "/")==0){
        return "../html/index.html";
    }else if(strcmp(route, "/bulky-page")==0){
        return "../html/bulky-webpage.html";
    }else{
        return "../html/404.html";
    }
}

char* get_html_page(char *route ){
    FILE *welcome_page=fopen(get_file_name_from_route(route), "r");
    if(welcome_page==NULL){
        printf("The corresponding HTML file for route %s not found:\n", route);
        return "<html><body>Error opening corresponding file</body></html>";
    }
    char *file_content;
    //Calculate Size of file
    fseek(welcome_page, 0, SEEK_END);
    long unsigned int size=ftell(welcome_page);
    file_content=(char *)malloc(size+10);
    fseek(welcome_page, 0, SEEK_SET);
    fread(file_content, 1, size, welcome_page);
    file_content[size]='\0';
    return file_content;
}