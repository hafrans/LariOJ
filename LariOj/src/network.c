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
#include <network.h>


const char *convertStructToJson(struct pop_info *pop,char *__dest){

    if(__dest == NULL)
        return NULL;

    sprintf(__dest,"\
{\n\
   solution_id : %d,\n\
   problem_id  : %d,\n\
   contest_id  : %d,\n\
   user_id     : %d,\n\
   status      : %d,\n\
   check_field : %d,\n\
}",pop->solution_id,pop->problem_id,pop->contest_id,pop->user_id,pop->solution_status,pop->check_key) ;

    return __dest;
}

void network_make_udp_call_to_webserver(const char *__addr,const int port,struct pop_info *pop){

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(__addr);
    addr.sin_port = htons(port);
    size_t len = sizeof(addr);
    char buf[1024];
    convertStructToJson(pop,buf);
    size_t body_len = strlen(buf);

    int sc = socket(AF_INET,SOCK_DGRAM,0);

    if(connect(sc,(struct sockaddr*)&addr,len) == 0){
       write(sc,buf,body_len);
    }

    close(sc);

}

