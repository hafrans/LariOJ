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
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

#ifndef __UTILITY_H_
#define __UTILITY_H_

    #ifdef __cplusplus

    extern "C" {

    #endif // __cplusplus

       extern int vsystem(const char* fmt,...);

       extern const char *trim(const char *text);

       extern void trim2(char *c);

       extern const char *read_ini(const char* filename,const char *name,char separator);

       extern int get_filesize(const char * file);

       extern const char *file_get_contents(char *_dest,size_t ___size,const char *file);

       extern void file_put_contents(const char *_dest_file,const char *data);

       extern int file_exists(const char *___file);

       extern char *strvcpy(char *__dest,char *__str__fmt,...);

       extern int init_lock(const char *lock_file);

       extern int remove_lock(const char *lock_file);

       extern void exit_program(void);

       extern void exitf(int __exit_code,...);

    #ifdef __cplusplus

    }

    #endif // __cplusplus



#endif // __UTILITY_H_
