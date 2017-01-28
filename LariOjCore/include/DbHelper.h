#include <mysql/mysql.h>
#include "stdio.h"
#include "string.h"
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
        virtual bool connect() throw (const char *);
        virtual bool connect(const char *__db__host_,
                           unsigned int  __db__port_,
                             const char *__db__user_,
                             const char *__db__pwd__,
                             const char *__db__dfl__dbn_) throw(const char *);
        virtual bool setdb(const char *__dbname) noexcept;
        virtual unsigned int get_errno() const noexcept;
        virtual const char *get_errmsg() const noexcept;
        virtual void set_sock(const char *sock)  noexcept;
        virtual unsigned int exec(const char *__cmd_query) throw(const char *);
        virtual vector< vector<std::string> > query(const char *__cmd_query) throw (const char *);
        virtual vector<std :: string > queryOneRow(const char *__cmd_query) throw (const char *);
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
