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

#include "deploy.h"

#ifndef STD_MB
#define STD_MB 1048576
#endif // STD_MB



int deploy_local(const char* solution_id,char* _local[]){

     pid_t deploy;
     int stat_val;


     deploy = fork();

     if(deploy < 0){

        zCritical("Can not deploy program in this server!");
        return -1;

     }else if(deploy == 0){

            /////////////////////////////////
            /// set Limits
            /////////////////////////////////

            struct rlimit LIM;

            LIM.rlim_max = LIM.rlim_cur = 900;//15 minutes
            setrlimit(RLIMIT_CPU,&LIM);

            LIM.rlim_max = LIM.rlim_cur = STD_MB * 512; //512MB
            setrlimit(RLIMIT_FSIZE,&LIM);

            LIM.rlim_max = LIM.rlim_cur = STD_MB * 256; //256MB
            setrlimit(RLIMIT_AS,&LIM);

            LIM.rlim_max = LIM.rlim_cur = 128;//128 cp
            setrlimit(RLIMIT_NPROC,&LIM);

            ///////////////////////////////////////////////

            char *args[] = {
            _local[5],"-d",_local[0],_local[1],_local[2],_local[3],_local[4],"-p",(char *)solution_id,(char *)0
            };


            if(!file_exists(_local[5])){
                exit(250);
            }

            execv(_local[5],args);

                exit(251);

     } else {

            wait(&stat_val);
            if(WIFEXITED(stat_val)){
                return WEXITSTATUS(stat_val);
            }
            if(WIFSIGNALED(stat_val)){
                return WTERMSIG(stat_val);
            }
            if(WIFSTOPPED(stat_val)){
                return WSTOPSIG(stat_val);
            }

     }

     return 0;

}


void deploy_delicated(const char *solution_id,char **_local){

     pid_t child = fork();

     if(child < 0){

        zCritical("Can not deploy program in this server!");
        return ;

     }else if(child == 0){

        int status = deploy_local(solution_id,_local);

        switch(status){
            case 0:
                zDebug("judged run solution ok\n");
                break;
            case 250:
                zCritical("JUDGER NOT FOUND\n");
                break;
            case 251:
                zCritical("JUDGER UNKNOWN ERROR!\n");
                break;
            default:
                zCritical("OTHER ERROR");
                break;
        }

        exit(0);

     }
}
























