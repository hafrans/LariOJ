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
int syscall_filiter[512] = {
    ///allowed system call
    #ifdef __i386
     SYS_time,
     SYS_read,
     SYS_uname,
     SYS_write,
     SYS_open,
     SYS_close,
     SYS_execve,
     SYS_access,
     SYS_brk,
     SYS_munmap,
     SYS_mprotect,
     SYS_mmap2,
     SYS_fstat64,
     SYS_set_thread_area,
     SYS_ioprio_get,
     SYS_exit_group,
     SYS_readlink,
     SYS_lseek

    #else
     SYS_time,
     SYS_read,
     SYS_uname,
     SYS_write,
     SYS_open,
     SYS_close,
     SYS_execve,
     SYS_access,
     SYS_brk,
     SYS_munmap,
     SYS_mprotect,
     SYS_mmap,
     SYS_fstat,
     SYS_set_thread_area,
     SYS_ioprio_get,
     SYS_arch_prctl,
     SYS_exit_group,
     SYS_readlink,
     SYS_lseek
    #endif // __i386


    ,500


};
