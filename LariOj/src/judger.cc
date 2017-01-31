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
#include "network.h"

//extern int __fi_lock = 0;

static DbHelper *__data;


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

       char buf[BUFF_SIZE];
       sprintf(buf,"update `solution` set status = %d where id = %d",soil->status,soil->id);
       __data != NULL && __data->exec(buf);

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
    char server_host [BUFF_SIZE] = {0};
    char server_port [BUFF_SIZE] = {0};



    struct solution soil;
    struct problem  prob;
    struct pop_info pop;







#ifndef DEBUG_MAIN

//    if(init_lock(NULL) == -1){
//        zCritical("this program is locked by /var/run/lari.lock \n you can try to remove the lock and then run this program\n");
//        return 60;
//    }
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

             if(offset < argc - 1)
             strcpy(mysql_host,args[++offset]);

        }else if(!strcmp("--d-p",args[offset])){

            if(offset < argc - 1)
            strcpy(mysql_port,args[++offset]);

        }else if(!strcmp("--s-h",args[offset])){

            if(offset < argc - 1)
            strcpy(server_host,args[++offset]);

        }else if(!strcmp("--s-p",args[offset])){

            if(offset < argc - 1)
            strcpy(server_port,args[++offset]);

        }else if(!strcmp("-p",args[offset])){

            if(offset < argc - 1)
            strcpy(solution_id,args[++offset]);

        }else if(!strcmp("--d-u",args[offset])){

            if(offset < argc - 1)
            strcpy(mysql_user,args[++offset]);

        }else if(!strcmp("--d-pwd",args[offset])){

            if(offset < argc - 1)
            strcpy(mysql_pwd,args[++offset]);

        }else if(!strcmp("--d-dbn",args[offset])){

            if(offset < argc - 1)
            strcpy(mysql_dbname,args[++offset]);

        }else if(!strcmp("-d",args[offset])){

            if(offset < argc - 1)
            strcpy(mysql_host,args[++offset]);
            if(offset < argc - 1)
            strcpy(mysql_port,args[++offset]);
            if(offset < argc - 1)
            strcpy(mysql_dbname,args[++offset]);
            if(offset < argc - 1)
            strcpy(mysql_user,args[++offset]);
            if(offset < argc - 1)
            strcpy(mysql_pwd,args[++offset]);
            else
            printf("\nmiss variable\n\n");

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
            puts("         --s-h                           report server host");
            puts("         --s-p                           report server port");
            puts("");
            return 0;
        }else{
             printf("\nUnknown option: %s\n\n",args[offset]);
             return 0;
        }


    }

        ////////////////////////////////////////////////////////////////////////////////////
        //check variables;

        if(!strcmp("",mysql_port))strcpy(mysql_port,"3306");
        if(!strcmp("",mysql_host))strcpy(mysql_host,"localhost");
        if(!strcmp("",mysql_user))strcpy(mysql_user,"root");
        if(!strcmp("",mysql_pwd))exitf(120,"mysql pwd is not specified\n");
        if(!strcmp("",solution_id))exitf(120,"solution is not specified.\n");



#else

    strcpy(mysql_host,"127.0.0.1");
    strcpy(mysql_port,"3306") ;
    strcpy(mysql_user,"root") ;
    strcpy(mysql_pwd,"root");
    strcpy(mysql_dbname,"test") ;
    strcpy(solution_id,"1") ;
    strcpy(server_host,"192.168.2.101") ;
    strcpy(server_port,"8888") ;

#endif //DEBUG_MAIN

        ////////////////////////////////////////////////////////////////////////////////////
        ///read configuration files

        ////////////////////////////////////////////////////////////////////////////////////

          DbHelper data(mysql_host,atoi(mysql_port),mysql_user,mysql_pwd,mysql_dbname);


          try{
            data.connect();
            __data = &data;
            char buf[BUFF_SIZE];
            sprintf(buf,"select * from solution where id = %s",solution_id);
            vector<std::string> vec = data.queryOneRow(buf);

            if(vec.size() <= 0)
                throw "no this solution !";

            soil.id         =  atoi(vec[0].c_str());
            soil.problem_id =  atoi(vec[1].c_str()) ;
            strcpy(soil.code,vec[8].c_str());
            soil.lang       =  static_cast<language>(atoi(vec[7].c_str()));
            soil.code_length=  strlen(soil.code);

            sprintf(buf,"select * from problem where id = %d",soil.problem_id);
            vector<std::string> vec2 = data.queryOneRow(buf);
            if(vec2.size() <= 0)
                throw "no this problem";

            prob.problem_id   = soil.problem_id;
            prob.lang_limit   = atoi(vec2[11].c_str());
            prob.mem_limit    = atoi(vec2[9].c_str());
            prob.time_limit   = atoi(vec2[8].c_str());
            prob.output_limit = atoi(vec2[10].c_str());
            prob.is_special   = atoi(vec2[4].c_str());



            /////////////////////////////////////////////////////
            judge_set_hook(functt);     //Register a hook
            judge_deploy(&soil,&prob);  //judge it
            /////////////////////////////////////////////////////



            //statistics

            pop.contest_id = atoi(vec2[1].c_str()); //contest
            pop.problem_id = soil.problem_id;
            pop.solution_id = soil.id;
            pop.solution_status = soil.status;
            pop.user_id = atoi(vec[3].c_str());
            pop.check_key = judge_hash(&soil);

            char          code[SO_CODE_L];
            char          errmsg[SO_STDERR_L];
            char          stdout[SO_STDOUT_L];

            data.escape(code,soil.code);
            data.escape(errmsg,soil.errmsg);
            data.escape(stdout,soil.stdout);

            sprintf(buf,"update solution set status = %d , used_time = %d , used_mem = %d , \
err_msg = '%s' ,stdout = '%s' , code_length = %d where id = %d ",soil.status,soil.used_time,soil.used_mem,errmsg,stdout,soil.code_length,soil.id);

            data.exec(buf);

            //other operations

            sprintf(buf,"update problem set submit = `submit` +1 where id = %d ",soil.problem_id);

            data.exec(buf);

            if(soil.status == ACCEPTED){

                sprintf(buf,"update problem set accepted = `accepted` +1 where id = %d",soil.problem_id);

                data.exec(buf);

            }
            //puts(buf);

          }catch(const char *errmsg){
            puts(errmsg);
          }



        ////////////////////////////////////////////////////////////////////////////////////

        if(strcmp("",server_host) && strcmp("",server_port)){
            network_make_udp_call_to_webserver(server_host,atoi(server_port),&pop);
        }

        ////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////




     remove_lock(NULL);


}








