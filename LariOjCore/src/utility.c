/* Copyright (C) 2009-2016 Hafrans <hafrans@gmail.com>

   This file is part of the LariOJ

          ___       ___           ___                       ___            ___
         /\__\     /\  \         /\  \          ___        /\  \          /\  \
        /:/  /    /::\  \       /::\  \        /\  \      /::\  \         \:\  \
       /:/  /    /:/\:\  \     /:/\:\  \       \:\  \    /:/\:\  \    ___ /::\__\
      /:/  /    /::\~\:\  \   /::\~\:\  \      /::\__\  /:/  \:\  \  /\  /:/\/__/
     /:/__/    /:/\:\ \:\__\ /:/\:\ \:\__\  __/:/\/__/ /:/__/ \:\__\ \:\/:/  /
     \:\  \    \/__\:\/:/  / \/_|::\/:/  / /\/:/  /    \:\  \ /:/  /  \::/  /
      \:\  \        \::/  /     |:|::/  /  \::/__/      \:\  /:/  /    \/__/
       \:\  \       /:/  /      |:|\/__/    \:\__\       \:\/:/  /
        \:\__\     /:/  /       |:|  |       \/__/        \::/  /
         \/__/     \/__/         \|__|                     \/__/

   LariOJ is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   LariOJ distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#include "utility.h"


#ifndef BUFF_SIZE
    #define BUFF_SIZE 2048
#endif // BUFF_SIZE

//declare some functions

int isspace(int);

int vsystem(const char *fmt,...){

    char cmd[BUFF_SIZE] = {0};

    va_list ap;
    va_start(ap,fmt);
    vsprintf(cmd,fmt,ap);
    va_end(ap);

    return system(cmd);

}

const char *trim(const char *text){

    char *top = (char *)calloc(strlen(text)+1,sizeof(char));
    strcpy(top,text);
    char *bottom = top+strlen(text)-1;

    for(;top<bottom && isspace(*top);++top);
    for(;top<=bottom&& isspace(*bottom);*bottom-- = 0);

    return top;

}

void trim2(char * c)
{
    char buf[BUFF_SIZE];
    char * start,*end;
    strcpy(buf,c);
    start=buf;
    while(isspace(*start)) start++;
    end=start;
    while(!isspace(*end)) end++;
    *end='\0';
    strcpy(c,start);
}

const char *read_ini(const char *filename,const char *name,char separator){
    char buf[BUFF_SIZE] = {0};     //bug hidden
    char *head = NULL;

    FILE *handle = fopen(filename,"r");

    if(!handle) return "N/a";
    while(fgets(buf,BUFF_SIZE,handle) != NULL && !feof(handle)){
        if(strstr(buf,name) != NULL){
           head = strchr(buf,separator);
           head != NULL && head++;
           trim2(head);
           break;
        }
    }

    fclose(handle);
    return head == NULL? "N/a" : head;
}


int get_filesize(const char* file)
{
    struct stat fstat;

    if (stat(file, &fstat) == -1)
        return -1;  //error

    return fstat.st_size;
}


const char *file_get_contents(char *_dest,size_t ___size,const char *file){
    if(_dest == NULL || file == NULL) return NULL;
    *_dest = 0;  //bug fix
    FILE *handle = fopen(file,"rb");
    if(!handle) return NULL;
    char buf[BUFF_SIZE];
    size_t __len = 0;
    size_t __size = 0;

    while(!feof(handle)){
       __len = fread(buf,sizeof(char),BUFF_SIZE-1,handle);
       buf[__len] = 0;
       if(__size+__len < ___size-1){
         strcat(_dest,buf);
       }else{
        break;
       }
       __size += __len;
    }
    _dest[__size] = 0;
    fclose(handle);
    return _dest;

}

void file_put_contents(const char *_dest_file,const char *data){
    if(data == NULL || _dest_file == NULL) return ;
    FILE *handle = fopen(_dest_file,"w");
    fputs(data,handle);
    fclose(handle);
}

int file_exists(const char *___file){
    FILE *handle = fopen(___file,"r");
    if(handle == NULL)
        return 0;
    else
        fclose(handle);
    return 1;
}

char *strvcpy(char *__dest,char *__str__fmt,...){
     char buf[BUFF_SIZE] = {0};

     va_list ap;
     va_start(ap,__str__fmt);

     vsprintf(buf,__str__fmt,ap);

     va_end(ap);

     return strcpy(__dest,buf);
}


int init_lock(const char *lock_file){
    char *__str_filename = "/var/run/lari.lock";
    if(lock_file != NULL){
       __str_filename = (char *)lock_file;
    }

    return open(__str_filename,O_CREAT | O_EXCL ,0755);
    //return file;

}

int remove_lock(const char *lock_file){
    char *__str_filename = "/var/run/lari.lock";
    if(lock_file != NULL){
       __str_filename = (char *)lock_file;
    }
    return unlink(__str_filename);
    //return remove(__str_filename);

}


void exit_program(void){
    remove_lock(NULL);
}

void exitf(int __exit_code,...){

    va_list ap;
    va_start(ap,__exit_code);

    if(__exit_code > 100){
        vfprintf(stderr,va_arg(ap,char*),ap);
    }

    va_end(ap);

    exit(__exit_code);
}














