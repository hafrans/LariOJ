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
      Lari OJ Core Header

     *** use "sudo useradd judger -u 811" to add a user for judger.
     *** create /home/LariOJ/judge for judger's workspace,
                /home/LariOJ/data  for stdin file & answers

     ====  these definitions can be modified in this Header file. ->Line : 92 ====

      Q:How to use this OJ Core ?
      A:In a nutshell,you should include this library in your program firstly.
        with that, you can deploy it with specified problem and solution (use struct),
        Finally it will return its results in struct .

        A SAMPLE is here:

        struct solution so = {0,65,0,0,"\
                        \
                        #include<stdio.h>\n\
                        int main(void)\n\
                        {\n\
                        printf(\"Hello %s World\\n\");\n\
                        return 0;\n\
                        }\n\
                        //\
                        ","","",QUEUING,COMP_LANG_C};
        struct problem prob = {65,20,50,2,0};

        judge_deploy(&so,&prob);


        printf("%d",so.status);
        printf("%s",so.errmsg);

        Q:What is magic hook ?
        A:when solution's status changed , a hook function will be called automatically .
          you can use the hook function you have declared to update solution or problem.

        Q:HOW TO USE MAGIC HOOK?
        A:Hook function is very easy to declare . A sample as follows :

            void* functt(struct solution *soil,struct problem *prob){

                if(soil != NULL){

                    printf("%d\n",soil->status);

                }
                return NULL;
            }
            and then, you should use judge_set_hook() to register it.


   **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>
#include <stdarg.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <asm/unistd.h>
#include <sys/user.h>
#include <sys/reg.h>
#include "utility.h"

#ifndef USER_HOME
#define USER_HOME "/home/LariOJ"
#define USER_HOME_DIR "/home/LariOJ/"
#define JUDGE_DIR "judge"
#define DATA_DIR  "data"
#define LARI_USER_UID 811
#endif // USER_HOME


#ifndef __JUDGE_H_
#define __JUDGE_H_

    #ifndef BUFF_SIZE
      #define BUFF_SIZE 2048
    #endif //

    #define STD_MB 1048576
    #define STD_KB 1024

    #define COMP_INFO "compiler_info.ret"

    #ifdef __i386      //x86
        #define SZ 4
        #define REG_VAL  long int
        #define REG_CALL ORIG_EAX
        #define REG_RET  EAX
        #define REG_ARG0 EBX
        #define REG_ARG1 ECX
    #else               //x86_64
        #define SZ 8
        #define REG_VAL  unsigned long long int
        #define REG_CALL ORIG_RAX
        #define REG_RET  RAX
        #define REG_ARG0 RDI
        #define REG_ARG1 RSI
     #endif

     //#define LARI_TEST



     #ifdef __cplusplus

     extern "C"{

     #endif // __cplusplus

     enum judge_status{
            QUEUING,
            COMPILING,
            COMPILE_ERROR,
            RUNNING,
            WRONG_ANSWER,
            ACCEPTED,
            RUNTIME_ERROR,
            SYSTEM_ERROR,
            PRESENTATION_ERROR,
            TIME_LIMIT_EXCEEDED,
            MEMORY_LIMIT_EXCEEDED,
            OUTPUT_LIMIT_EXCEEDED,
            SECURITY_ERROR,
            UNKNOWN_ERROR
     };

     enum language {
            COMP_LANG_C,
            COMP_LANG_CXX,
            //COMP_LANG_JAVA,
            //COMP_LANG_PA
     };

     struct solution{
            int           id;
            int           problem;
            int           used_time; //ms
            int           used_mem;  //b
            char          code[STD_KB*64];
            char          errmsg[STD_KB*4];
            char          stdout[STD_KB*4];
            enum          judge_status status;
            enum          language     lang;
     };


     struct problem{
            int          problem_id;
            int          time_limit;
            int          mem_limit;
            int          output_limit;
            int          lang_limit;
    };







     extern void judge_run(struct solution *soil,struct problem *prob,const char *workspace);

     extern void judge_deploy(struct solution *soil,struct problem *prob);

     extern void judge_watchdog(pid_t cpid,struct solution *soil,struct problem *prob);

     extern void judge_set_hook(void *(* func)(struct solution *,struct problem *));

     extern int  judge_compile(struct solution *soil,struct problem *prob);

     extern int  judge_check_mem_peak(pid_t cpid) ;

     extern void inline judge_env_creat(int solution_id,char *work_space,struct solution *soil);

     extern void inline judge_env_destruct(char *workspace);

     extern void inline judge_prepare_env(struct problem *prob,const char *workspace);

     extern void inline judge_env_shell_creat(const char *workspace);





     #ifdef __cplusplus

     }

     #endif // __cplusplus





#endif // __JUDGE_H_







