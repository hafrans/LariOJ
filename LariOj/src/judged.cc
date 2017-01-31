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

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

#include "lari.h"
#include "unistd.h"
#include "sys/wait.h"
#include "sys/types.h"
#include "network.h"
#include "DbHelper.h"
#include "utility.h"
#include "deploy.h"


using std::vector;

void exit_handle(int sig){


    fprintf(stderr,"\n\n\nPROGRAM EXIT (%d)\n",sig);
    exit(0);

}


int main(int argc,char *argv[])
{

    conf_t ct = open_conf_file(DFL_CONF_PATH);

    char mysql_host  [BUFF_SIZE] = {0};
    char mysql_user  [BUFF_SIZE] = {0};
    char mysql_pwd   [BUFF_SIZE] = {0};
    char mysql_dbname[BUFF_SIZE] = {0};
    char mysql_port  [BUFF_SIZE] = {0};
    char server_type [BUFF_SIZE] = {0};
    char buf         [BUFF_SIZE] = {0};
//    int  server_type = 0;


    ///////////////////////////////////////
    ///   DataBase Configuration
    //////////////////////////////////////
    read_conf(ct,"server_host",mysql_host);
    read_conf(ct,"server_port",mysql_port);
    read_conf(ct,"server_user",mysql_user);
    read_conf(ct,"server_pwd" ,mysql_pwd);
    read_conf(ct,"server_dbn" ,mysql_dbname);
    read_conf(ct,   "type"    ,server_type);


    char *local_args[] = {
    mysql_host,mysql_port,mysql_dbname,mysql_user,mysql_pwd
    }  ;


    ///////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////



    //read_conf(ct,"server_host",mysql_host);

    ///Register signal handler
    signal(SIGABRT,exit_handle);
    signal(SIGINT ,exit_handle);
    signal(SIGTERM,exit_handle);
    signal(SIGTRAP,exit_handle);
    signal(SIGHUP ,exit_handle);
    signal(SIGKILL,exit_handle);
    signal(SIGQUIT,exit_handle);
    signal(SIGSTOP,exit_handle);


    DbHelper data(mysql_host,atoi(mysql_port),mysql_user,mysql_pwd,mysql_dbname);

    try
    {

        data.connect();//db connection

        if(!strcmp("delicated",server_type)){

           // printf("%lf",get_loadavg(&a)->_load_1);

            while(1){

                vector<std::string> mVector = data.queryOneRow("select distinct `id` from solution where `status` = 0 order by `time` asc");

                if(mVector.empty()){
                    zDebug("waiting for new solution..\n");
                }

                for(vector<std::string> :: const_iterator it = mVector.cbegin(); it != mVector.cend();++it){

                    zDebug("Get Solution id : %s from DataBase.\n",it->c_str());
                    sprintf(buf,"update `solution` set `status` = 1 where id %s",it->c_str());
                    data.exec(buf);

                    //deploy

                    deploy_delicated(it->c_str(),local_args);


                    zDebug("Solution id : %s deployed +--> LocalHost.\n",it->c_str());
                    sleep(1);

                }



                sleep(3);
            }


        }else if(!strcmp("master",server_type)){

            throw "master type is not allowed" ;


        }else if(!strcmp("slave",server_type)){

            throw "slave type is not allowed" ;

        }else{
            throw "***CONF :server type error!";
        }





    }
    catch(const char *errmsg)
    {


        fprintf(stderr,"\n\nERROR:%s\n",errmsg);

    }




    return 0;

}
