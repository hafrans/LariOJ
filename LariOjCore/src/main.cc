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

            /**
                Main File
                            **/
#include "DbHelper.h"
#include "lari.h"
#include "utility.h"
#include "judge_core.h"
#define DEBUG_MAIN


//extern int __fi_lock = 0;


/**
    struct solution in judge_core.h :

        struct solution{
            int           id;
            int           problem_id;
            int           used_time; //ms
            int           used_mem;  //b
            char          code[STD_KB*64];
            char          errmsg[STD_KB*4];
            char          stdout[STD_KB*4];
            enum          judge_status status;
            enum          language     lang;
     };

     struct problem in judge_core.h:

        struct problem{
            int          problem_id;
            int          time_limit;
            int          mem_limit;
            int          output_limit;
            int          lang_limit;
        };
    **/

void* functt(struct solution *soil,struct problem *prob){

    if(soil != NULL){

        //puts("WE GET A CALL");
        printf("%d\n",soil->status);

    }
    return NULL;
}


int main(int argc,char *args[]){


    char mysql_host  [BUFF_SIZE] = {0};
    char mysql_user  [BUFF_SIZE] = {0};
    char mysql_pwd   [BUFF_SIZE] = {0};
    char mysql_dbname[BUFF_SIZE] = {0};
    char mysql_port  [BUFF_SIZE] = {0};
    char solution_id [BUFF_SIZE] = {0};

   // judge_set_hook(functt);

#ifndef DEBUG_MAIN

    if(init_lock(NULL) == -1){
        zCritical("this program is locked by /var/run/lari.lock \n you can try to remove the lock and then run this program\n");
        return 60;
    }
    atexit(LARI_EXIT);

    if(argc == 1){
            puts("LariOJ -- version 0.9.6 ");
            puts("\nLariOJ is a Online Judge System which is very\nconvenient to deploy.\n");
            puts("Usage: judger [options] command\n");
            puts("use \"--help\" to get more help.");
            return 0;
    }
    for(int offset = 1;offset < argc;++offset){

        if(!strcmp("--d-h",args[offset])){
             strcpy(mysql_host,args[offset++]);
        }else if(!strcmp("--d-p",args[offset])){
            strcpy(mysql_port,args[offset++]);
        }else if(!strcmp("-p",args[offset])){
            strcpy(solution_id,args[offset++]);
        }else if(!strcmp("--d-u",args[offset])){
            strcpy(mysql_user,args[offset++]);
        }else if(!strcmp("--d-pwd",args[offset])){
            strcpy(mysql_pwd,args[offset++]);
        }else if(!strcmp("--d-dbn",args[offset])){
            strcpy(mysql_dbname,args[offset++]);
        }else if(!strcmp("-d",args[offset])){
            strcpy(mysql_host,args[offset++]);
            strcpy(mysql_port,args[offset++]);
            strcpy(mysql_dbname,args[offset++]);
            strcpy(mysql_user,args[offset++]);
            strcpy(mysql_pwd,args[offset++]);
        }else if(!strcmp("--help",args[offset]) || !strcmp("--h",args[offset])){
            printf("Usage: %s [options] command\n\n",args[0]);
            puts("Arguments:\n");
            puts("         --d-h   [host=localhost]        mysql host");
            puts("         --d-p   [port=3306]             mysql port");
            puts("         --d-u   [user=root]             mysql usrname");
            puts("         --d-pwd [pwd =root]             mysql password");
            puts("         --d-dbn [dbn ]                  database name");
            puts("          -d [h] [p] [dbn] [u] [pwd]     config");
            puts("          -p                             solution id in database");
            puts("");
        }else{
             printf("\nUnknown option: %s\n\n",args[offset]);
             return 0;
        }

        ////////////////////////////////////////////////////////////////////////////////////
        //check variables;

        if(!strcmp("",mysql_port))strcpy(mysql_port,"3306");
        if(!strcmp("",mysql_host))strcpy(mysql_host,"localhost");
        if(!strcmp("",mysql_user))strcpy(mysql_user,"root");
        if(!strcmp("",mysql_pwd))exitf(120,"mysql pwd is not specified\n");
        if(!strcmp("",solution_id))exitf(120,"solution is not specified.\n");

    }

#else

    strcpy(mysql_host,"127.0.0.1");
    strcpy(mysql_port,"3306") ;
    strcpy(mysql_user,"root") ;
    strcpy(mysql_pwd,"root");
    strcpy(mysql_dbname,"test") ;

#endif //DEBUG_MAIN

        ////////////////////////////////////////////////////////////////////////////////////

          DbHelper data(mysql_host,atoi(mysql_port),mysql_user,mysql_pwd,mysql_dbname);

          try{
            data.connect();
            vector<std::string> vec = data.queryOneRow("select * from h_article");
             printf("%s ",vec[3].c_str());
//            vector<vector<std::string> > vec = data.query("select * from h_article");
//
//            for(vector<vector<std::string> >::iterator it = vec.begin();it != vec.end();++it){
//                for(vector<std::string>::iterator _it = it->begin(); _it != it->end();++_it){
//                    printf("%s ",_it->c_str());
//                }
//                printf("\n");
//            }
          }catch(const char *errmsg){
            puts(errmsg);
          }



//        MYSQL *handle = mysql_init(NULL);
//
//        mysql_real_connect(handle,"127.0.0.1","root","","test",0,NULL,0);
//
         //puts(data.get_errmsg());

        ////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////




     remove_lock(NULL);


}








