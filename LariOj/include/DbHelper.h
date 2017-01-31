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

#include <mysql.h>
#include <cstdio>
#include <cstring>
#include <cstdarg>
#include <vector>
#include <string>
#include <iostream>


/****

         //query
          DbHelper data(mysql_host,atoi(mysql_port),mysql_user,mysql_pwd,mysql_dbname);

          try{
            data.connect();
            vector<vector<std::string> > vec = data.query("select * from h_article");

            for(vector<vector<std::string> >::iterator it = vec.begin();it != vec.end();++it){
                for(vector<std::string>::iterator _it = it->begin(); _it != it->end();++_it){
                    printf("%s ",_it->c_str());
                }
                printf("\n");
            }
          }catch(const char *errmsg){
            puts(errmsg);
          }




***/

using std::vector;
using std::string;

#ifndef DBHELPER_H
#define DBHELPER_H



class DbHelper
{
    public:
        virtual  bool                          connect() throw (const char *);
        virtual  bool                          connect(const char *__db__host_,
                                                     unsigned int  __db__port_,
                                                       const char *__db__user_,
                                                       const char *__db__pwd__,
                                                       const char *__db__dfl__dbn_) throw(const char *);
        virtual  bool                          setdb(const char *__dbname) noexcept;
        virtual  unsigned int                  get_errno() const noexcept;
        virtual  const char    *               get_errmsg() const noexcept;
        virtual  void                          set_sock(const char *sock)  noexcept;
        virtual  unsigned int                  exec(const char *__cmd_query) throw(const char *);
        virtual  vector< vector<std::string> > query(const char *__cmd_query) throw (const char *);
        virtual  vector<std :: string >        queryOneRow(const char *__cmd_query) throw (const char *);
        virtual  unsigned long                 escape(char *__str__dest,const char *__str_src) noexcept;
                 const char    *               get_host() const;
                 unsigned int                  get_port() const;
                 const char    *               get_user() const;
                 const char    *               get_pwd () const;
                 const char    *               get_dbn () const;


        explicit DbHelper();
        DbHelper(char *__db__host_,
               unsigned int  __db__port_,
                 char *__db__user_,
                 char *__db__pwd__,
                 char *__db__dfl__dbn_):
                       __db__host_(__db__host_),
                       __db__port_(__db__port_),
                       __db__user_(__db__user_),
                       __db__pwd__(__db__pwd__),
                       __db__dfl__dbn_(__db__dfl__dbn_){handle=NULL;__db__sock_=NULL;};
        virtual ~DbHelper();

    protected:
                char *__db__host_;
        unsigned int  __db__port_;
                char *__db__user_;
                char *__db__pwd__;
                char *__db__dfl__dbn_;
                char *__db__sock_;



        MYSQL *handle;
        MYSQL_RES *result;
        MYSQL_ROW rows;


};

#endif // DBHELPER_H
