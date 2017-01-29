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

#include "judge_core.h"
#include "syscall_filiter.h"
#include "debug.h"

static void *(*lari_hook)(struct solution *,struct problem *) = NULL;


int internal_syscall_filiter[512];


void judge_set_hook(void *(* func)(struct solution *,struct problem *)){

    lari_hook = func;
}



void judge_init_syscall_filiter(){
    int i = 0;
    for(;syscall_filiter[i]!=500&&i<512;++i){
       internal_syscall_filiter[syscall_filiter[i]] = -1;
    }
}


void inline judge_prepare_env(struct problem *prob,const char *workspace){

    char buf[BUFF_SIZE] = {0};
    //copy the question items file(s);
    sprintf(buf,"%s/data/%d/read.in",USER_HOME,prob->problem_id);

    if(file_exists(buf)){
        #ifdef DEBUG
        zDebug("%s found , copy it to workspace\n",buf);
        #endif // DEBUG
        vsystem("cp %s ./",buf);
    }else{
        #ifdef DEBUG
        zDebug("%s not found\n",buf);
        #endif // DEBUG
    }

    return ;
}


void judge_deploy_user_program(const char *program,enum language lang){

    char *buf = NULL;

    switch(lang){
        case COMP_LANG_C:
            buf = "main.c";
            break;
        case COMP_LANG_CXX:
            buf = "main.cc";
            break;
    }

    file_put_contents(buf,program); //

}





int  judge_check_mem_peak(pid_t cpid){

    char buf [BUFF_SIZE] = {0};
    const char *result = NULL;

    sprintf(buf,"/proc/%d/status",cpid);
    result = read_ini(buf,"VmPeak",':');
   // puts(result);
    if(!strcmp("N/a",result))
        result = "-1";
    return atoi(result);

}

void inline judge_env_shell_creat(const char *workspace){

    #ifdef DEBUG
    zDebug("creating sandbox for the solution .\n");
    #endif // DEBUG
    vsystem("mkdir lib && mkdir lib64");
    vsystem("mkdir lib32");
    vsystem("mkdir bin && mkdir etc && mkdir -p usr/bin");

    vsystem("cp /bin/busybox ./bin/");
    vsystem("ln -s /bin/busybox ./bin/sh"); //sh
    vsystem("cp /bin/bash ./bin/bash");

    vsystem("cp -f /lib/* ./lib/ >/dev/null 2>&1 ");
    vsystem("cp -f /lib32/* ./lib32/ >/dev/null 2>&1  ");
    vsystem("cp -f /lib64/* ./lib64/ >/dev/null 2>&1 ");
    vsystem("cp -a /lib/*-linux-gnu ./lib/ > /dev/null 2>&1 ");

    //cp some functions
    //vsystem("ln -s /bin/busybox ./bin/traceroute");

    #ifdef DEBUG
    zDebug("sandbox is ok .\n");
    #endif // DEBUG

}

void judge_accept(struct solution *__strict_soil, struct problem *__strict_prob){
    if(__strict_soil == NULL || __strict_prob == NULL){
        zCritical("Can not judge this solution!");
        exit(65);
    }
    char __str_answer[512] = {0};
    char __str_solution[512] = {0};

    char __c__answer   = 0;
    char __c__solution = 0;
    int  __if__accept  = 0;//PRESENTATION_ERROR?;


    sprintf(__str_answer  ,"%s%s/%d/data.out",USER_HOME_DIR,DATA_DIR ,__strict_prob->problem_id);
    sprintf(__str_solution,"%s%s/%d/user.out",USER_HOME_DIR,JUDGE_DIR,__strict_soil->id);

    FILE *__fh_answer   = fopen(__str_answer  ,"r");
    FILE *__fh_solution = fopen(__str_solution,"r");
    #ifdef DEBUG
    if(__fh_answer == NULL){
        zDebug("%s is not found!\n",__str_answer);
    }
    if(__fh_solution == NULL){
        zDebug("%s is not found \n!",__str_solution);
    }
    #endif // DEBUG

    if(__fh_answer == NULL || __fh_solution == NULL){
        zCritical("Can not judge this solution %d,problem %d __fh_answer or __fh_solution\n",__strict_soil->id,__strict_prob->problem_id);
        __strict_soil->status = SYSTEM_ERROR;
        lari_hook != NULL ?  lari_hook(__strict_soil,__strict_prob) : NULL;
        return ;
    }//if

    __strict_soil->status = JUDGING;
    lari_hook != NULL ?  lari_hook(__strict_soil,__strict_prob) : NULL;

    __c__answer   = fgetc(__fh_answer  );
    __c__solution = fgetc(__fh_solution);
    for(;;__c__answer = fgetc(__fh_answer),__c__solution = fgetc(__fh_solution)){//check . one by one
           //__c__answer = fgetc(__fh_answer),__c__solution = fgetc(__fh_solution)
           while(isspace(__c__answer) || isspace(__c__solution)){


                if((__c__answer == '\n' && __c__solution == EOF) ||
                   (__c__answer == EOF && __c__solution == '\n'))
                    break;
                if(__c__answer == EOF && __c__solution == EOF) {
                    break;
                }
                if((isspace(__c__answer) && !isspace(__c__solution)) ||
                   (!isspace(__c__answer) && isspace(__c__solution))){
                    __if__accept |= PRESENTATION_ERROR;
                }


                if(isspace(__c__answer)){
                    __c__answer = fgetc(__fh_answer);
                }

                if(isspace(__c__solution)){
                    __c__solution = fgetc(__fh_solution);
                }
           }
           if((__c__answer == '\n' && __c__solution == EOF) ||
              (__c__answer == EOF && __c__solution == '\n'))
               break;
           if(__c__answer == EOF && __c__solution == EOF){
               break;
           }
           if(__c__answer != __c__solution){
               __if__accept |= WRONG_ANSWER;
               break;
           }


    } //for
    if(__if__accept & WRONG_ANSWER){
        __strict_soil->status  = WRONG_ANSWER;
    }else if(__if__accept & PRESENTATION_ERROR){
        __strict_soil->status  = PRESENTATION_ERROR;
    }else{
        __strict_soil->status = ACCEPTED;
    }

    //hook
    lari_hook != NULL ?  lari_hook(__strict_soil,__strict_prob) : NULL;

    fclose(__fh_answer);
    fclose(__fh_solution);
}

void inline judge_env_creat(int solution_id,char *work_space,struct solution *soil){
      if(work_space == NULL || soil == NULL){
        zCritical("can not create workspace !\n");
        exit(65);
      }

      sprintf(work_space,"%s%s/%d",USER_HOME_DIR,JUDGE_DIR,solution_id);

      #ifdef DEBUG
      zDebug("set work dir %s\n",work_space);
      #endif // DEBUG
      mkdir(USER_HOME_DIR,0755);
      if(mkdir(work_space,0755) == -1){
        zCritical("create %s failed ! Permission Denied.\n",work_space);
        soil->status = SYSTEM_ERROR;
        strcpy(soil->errmsg,"can not create workspace because of duplicated solution.") ;
        ///hook
        lari_hook != NULL ?  lari_hook(soil,NULL) : NULL;
        return ;
      }

      chdir(work_space);

      //prepare a sandbox
      judge_env_shell_creat(work_space);


}

void inline judge_env_destruct(char *workspace){

    #ifdef DEBUG
    zDebug("cleaning workspace %s ..\n",workspace);
    #endif // DEBUG
    vsystem("rm -Rf %s",workspace);

}


int judge_compile(struct solution *soil,struct problem *prob){

      if(soil == NULL || prob == NULL){
        zCritical("can not get a valid pointer !\n");
        exit(63);
      }

      int stat_val;

      char buf[BUFF_SIZE];

      char * LANGPAK_C  [] = { "gcc", "main.c", "-o", "main", "-O2","-w", "-lm", //c
                            "--static", "-std=c99", "-DONLINE_JUDGE","-DLARI_OJ", NULL
                          };
      char * LANGPAK_CXX[] = { "g++", "main.cc", "-o", "main","-O2", "-w",       // c++
                            "-lm", "--static", "-DONLINE_JUDGE", "-DLARI_OJ" ,NULL
                          };

      pid_t compilerChild = fork();

      if(compilerChild < 0 ){
        zCritical("can not create a compiler process!\n");
        exit(64);

      } else if(compilerChild == 0 ){
         #ifdef DEBUG
         zDebug("compile the program .. \n");
         #endif // DEBUG


        //////////////////////////compile//////////////////
        ////   Limitation
        ///////////////////////////////////////////
        struct rlimit LIM;
        LIM.rlim_max = LIM.rlim_cur = 30; //less than 30 seconds
        setrlimit(RLIMIT_CPU, &LIM);

        LIM.rlim_max = LIM.rlim_cur = 100 * STD_MB; //less than 100MB
        setrlimit(RLIMIT_FSIZE, &LIM);

        LIM.rlim_max = LIM.rlim_cur = 64 * STD_MB;  //less than 64MB
        setrlimit(RLIMIT_AS, &LIM);


        freopen(COMP_INFO,"w",stderr); //redirect to a file


        soil->status = COMPILING;

        ///hook
        lari_hook != NULL ?  lari_hook(soil,prob) : NULL;


        //////////////////////////////////////////////////////////
        switch(soil->lang){
            case COMP_LANG_C:
                execvp(LANGPAK_C[0],LANGPAK_C);
                break;
            case COMP_LANG_CXX:
                execvp(LANGPAK_CXX[0],LANGPAK_CXX)  ;
                break;
        }
        ///////////////////////////////////////////////////////////




      } else{

        waitpid(compilerChild,NULL,0);
        #ifdef DEBUG
        zDebug("compilation finished\n");
        #endif // DEBUG

        stat_val = get_filesize(COMP_INFO);
        if(stat_val != 0 ){
            soil->status = COMPILE_ERROR;
            strcpy(soil->errmsg,file_get_contents(buf,BUFF_SIZE,COMP_INFO)) ;
            ///hook
            lari_hook != NULL ?  lari_hook(soil,prob) : NULL;
            #ifdef DEBUG
            zDebug("%s\n",buf);
            #endif // DEBUG
        }

         return stat_val;
      }

     return -1;

}




void judge_run(struct solution *soil,struct problem *prob,const char *workspace){  //should be forked

    if(soil == NULL){
        #ifdef CRITICAL
        zCritical("soil handler is null,judge failed.\n") ;
        #endif // CRITICAL
        exit(64);
    }

    ///soil->status = RUNNING;
    ///hook

    //vsystem("traceroute 23.88.238.66") ;


    ///redirect std streams
    #ifdef CRITICAL
     zDebug("redirecting std streams \n","s");
    #endif // CRITICAL

    freopen("read.in", "r+", stdin);

    freopen("user.out", "w", stdout);

    freopen("error.out", "a+", stderr);


    // set the priority
    nice(18);

    setuid(LARI_USER_UID);//judge user run







    ////////////////////////////////////////////
    ////   Limitation
    ///////////////////////////////////////////
    struct rlimit LIM;

    LIM.rlim_max = LIM.rlim_cur = prob->time_limit + 1 ;//set cpu time limit

    setrlimit(RLIMIT_CPU,&LIM); // if time limit exceeded,it will raise a SIGXCPU


    LIM.rlim_cur = prob->output_limit * STD_KB;
    LIM.rlim_max = LIM.rlim_cur + STD_MB;

    setrlimit(RLIMIT_FSIZE,&LIM);//if output limit exceeded , it will raise a SIGXFSZ


    LIM.rlim_cur = STD_MB << 6;
    LIM.rlim_max = STD_MB << 6;

    setrlimit(RLIMIT_STACK, &LIM);// set the stack size


    LIM.rlim_cur = STD_MB * prob->mem_limit * 2;
    LIM.rlim_max = STD_MB * prob->mem_limit * 3;

    setrlimit(RLIMIT_AS, &LIM);// set the memory

    LIM.rlim_max = LIM.rlim_cur = 1;//only one process

    setrlimit(RLIMIT_NPROC, &LIM);


    ////////////
    // If program is sleeping,this alarm clock will wake him up !
    ///////////
    alarm(0);
    alarm(prob->time_limit * 3);


    ptrace(PTRACE_TRACEME,0,NULL,NULL);
    //run now

    #ifdef LARI_TEST
    execl("/home/hafrans/Desktop/PlexClient/bin/Debug/PlexClient","/home/hafrans/Desktop/PlexClient/bin/Debug/PlexClient",NULL);
    #else
    switch(soil->lang){
        case COMP_LANG_C:
        case COMP_LANG_CXX:
            execl("./main","./main",NULL);
        default :
            break;//raise a error
    }
    #endif // LARI_TEST


    exit(251);

}

void judge_watchdog(pid_t cpid,struct solution *soil,struct problem *prob){

    if(soil == NULL){
        #ifdef CRITICAL
        zCritical("soil handler is null,judge failed.\n") ;
        #endif // CRITICAL
        exit(67);
    }
    int stat_val;
    struct rusage ruse;
    //int used_time = 0;
    int used_mem = 0;
    soil->status = RUNNING;
    lari_hook != NULL ?  lari_hook(soil,prob) : NULL;
    #ifdef DEBUG
    zDebug("a dog is looking %d\n",cpid);
    #endif // DEBUG
    while(1){
       wait4(-1,&stat_val,0,&ruse);

       ///runtime
       if(judge_check_mem_peak(cpid) > prob->mem_limit*STD_KB){
            #ifdef DEBUG
            zDebug("child process %d caused a MLE (larger than %d KB)\n",cpid,prob->mem_limit);
            #endif // DEBUG
            ptrace(PTRACE_KILL,cpid,NULL,NULL);
            soil->status = MEMORY_LIMIT_EXCEEDED;
            strcpy(soil->errmsg,"Out of Memory.");
            //lari_hook != NULL ?  lari_hook(soil,prob) : NULL;
            break;
       }

       ////exit

       if(WIFEXITED(stat_val)){ //exit without exception
            #ifdef DEBUG
             zDebug("child process %d exited with code %d \n",cpid,WEXITSTATUS(stat_val));
            #endif // DEBUG
          if(WEXITSTATUS(stat_val) == 251){
            #ifdef DEBUG
             zDebug("Caused a System Error!\n");
            #endif // DEBUG
            #ifdef DEBUG
             zCritical("the program lost its files! \n");
            #endif // DEBUG
            soil->status = SYSTEM_ERROR;
            strcpy(soil->errmsg,"the program lost its files! \n");
          }

          break;

       }else if(WIFSIGNALED(stat_val)){

            /*  WIFSIGNALED: if the process is terminated by signal
             *
             *  5  means Trace/breakpoint trap
             *  11 means Segmentation fault
             *  25 means File size limit exceeded
             */


           #ifdef DEBUG
           zDebug("judge process %d caused unexcepted error :",cpid,WEXITSTATUS(stat_val));
           #endif // DEBUG

         switch(WTERMSIG(stat_val)){
                case SIGCHLD:
                case SIGALRM:
                    #ifdef DEBUG
                    fprintf(stderr,"SLEEP\n");
                    #endif // DEBUG
                    alarm(0);
                case SIGKILL:
                case SIGXCPU:
                    #ifdef DEBUG
                    fprintf(stderr,"TLE (larger than %u s)\n",prob->time_limit);
                    #endif // DEBUG
                    soil->status = TIME_LIMIT_EXCEEDED;
                    strcpy(soil->errmsg,"Time Limit Exceeded.");
                    break;
                case SIGXFSZ:
                    #ifdef DEBUG
                    fprintf(stderr,"OLE(larger than %u k)\n",prob->output_limit);
                    #endif // DEBUG
                    soil->status = OUTPUT_LIMIT_EXCEEDED;
                    strcpy(soil->errmsg,"Output Limit Exceeded");
                    break;
                default:
                    #ifdef DEBUG
                    fprintf(stderr,"RE [%d]\n",WTERMSIG(stat_val));
                    #endif // DEBUG
                    soil->status = RUNTIME_ERROR;
                    file_get_contents(soil->errmsg,4096,"error.out");
         } //switch
         ptrace(PTRACE_KILL,cpid,NULL,NULL);
         break;

       } else if(WSTOPSIG(stat_val) != 5){  //means not Trace/breakpoint trap
               /**
                *  maybe we can not get a SIGFSZ or SIGSEGV by WTERMSIG()
                *  nowthat you can use WSTOPSIG to catch it.
                *  Attention: 5 is a trap/breakpoint
                */

               #ifdef DEBUG
               zDebug("judge process %d caused unexcepted error :",cpid,WEXITSTATUS(stat_val));
               #endif // DEBUG

                 switch(WSTOPSIG(stat_val)){
                        case SIGCHLD:
                        case SIGALRM:
                            #ifdef DEBUG
                            fprintf(stderr,"SLEEP\n");
                            #endif // DEBUG
                            alarm(0);
                        case SIGKILL:
                        case SIGXCPU:
                            #ifdef DEBUG
                            fprintf(stderr,"TLE (larger than %u s)\n",prob->time_limit);
                            #endif // DEBUG
                            soil->status = TIME_LIMIT_EXCEEDED;
                            strcpy(soil->errmsg,"Time Limit Exceeded.");
                            break;
                        case SIGXFSZ:
                            #ifdef DEBUG
                            fprintf(stderr,"OLE(larger than %u k)\n",prob->output_limit);
                            #endif // DEBUG
                            soil->status = OUTPUT_LIMIT_EXCEEDED;
                            strcpy(soil->errmsg,"Output Limit Exceeded.");
                            break;
                        default:
                            #ifdef DEBUG
                            fprintf(stderr,"RE [%d]\n",WSTOPSIG(stat_val));
                            #endif // DEBUG
                            soil->status = RUNTIME_ERROR;
                            file_get_contents(soil->errmsg,4096,"error.out");
                 } //switch

                 ptrace(PTRACE_KILL,cpid,NULL,NULL);
                 break;

       }else{//if(stop == 5)

            REG_VAL regVal = ptrace(PTRACE_PEEKUSER,cpid,SZ*REG_CALL);
            if(!internal_syscall_filiter[regVal]){

                 #ifdef CRITICAL
                  zCritical("a system call id: %d is not allowed ! - LariSystem\n",regVal);
                 #endif // CRI
                 soil->status = SECURITY_ERROR;
                 strvcpy(soil->errmsg,"Security Error : a system call id : %d is not allowed.",regVal);
                 ptrace(PTRACE_KILL,cpid,NULL,NULL);
                 break;
            }




       } //if









       used_mem = judge_check_mem_peak(cpid);
       if(used_mem > soil->used_mem)
        soil->used_mem = used_mem;
       ///next step
       ptrace(PTRACE_SYSCALL, cpid, NULL, NULL);

    }

    soil->used_time += (ruse.ru_utime.tv_sec * 1000 + //user time
                        ruse.ru_utime.tv_usec / 1000);//xSyyz
    soil->used_time += (ruse.ru_stime.tv_sec * 1000 + //system time
                        ruse.ru_stime.tv_usec / 1000);

    #ifdef DEBUG
    zDebug("running process %d OK , time (%d) , mem (%d)\n",cpid,soil->used_time,soil->used_mem);
    #endif // DEBUG
    ///hook
    if(soil->status != RUNNING){
         lari_hook != NULL ?  lari_hook(soil,prob) : NULL;
    }
}

void judge_deploy(struct solution *soil,struct problem *prob){

    char work_space[BUFF_SIZE];

    if(soil == NULL){
        #ifdef CRITICAL
        zCritical("Deploy failed because of soil nullptr\n") ;
        #endif // CRI
        return ;
    }

    judge_init_syscall_filiter();

    judge_env_creat(soil->id,work_space,soil);  ///create environment for judger

    if(soil->status != SYSTEM_ERROR){
        judge_prepare_env(prob,work_space);
    }


    //printf("%d",soil->status);


    if(soil->status < COMPILING)
    {
        judge_deploy_user_program(soil->code,soil->lang);

        if(judge_compile(soil,prob)){

            #ifdef DEBUG
            zDebug("compilation failed.\n");
            #endif
           // return ;
        }  ///complier  run
    }







    if(soil->status < COMPILE_ERROR){
        pid_t cpid = fork();

        if(cpid < 0){
            fprintf(stderr,"Can not Deploy a new Task\n");
            exit(64);
        }else if(cpid == 0){
            chroot(work_space);
            judge_run(soil,prob,work_space);
        }else{
            judge_watchdog(cpid,soil,prob);
            //copy the results & errors
            file_get_contents(soil->stdout,SO_STDOUT_L,"user.out");
            if(prob->problem_id != 0 && soil->status == RUNNING){
            // TODO (hafrans#1#): check the program's answer.
              //zDebug("problem %d is Accepted.\n",prob->problem_id);
              judge_accept(soil,prob);
              #ifdef DEBUG
              if(soil->status == WRONG_ANSWER){
                zDebug("solution %d is WRONGANSWER.\n",prob->problem_id);
              }else if(soil->status == PRESENTATION_ERROR ){
                zDebug("solution %d is Presentation Error.\n",prob->problem_id);
              }else if(soil->status == ACCEPTED){
                  zDebug("solution %d is Accepted.\n",prob->problem_id);
              }
              #endif // DEBUG
            }else{
                soil->status = USER_SUBMIT;
                lari_hook != NULL ?  lari_hook(soil,prob) : NULL;
            }
        }
    }



    judge_env_destruct(work_space);  //clean the workspace

}

















