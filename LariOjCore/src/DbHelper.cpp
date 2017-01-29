#include "DbHelper.h"

DbHelper::DbHelper()
{
    handle = NULL;
    __db__dfl__dbn_ = NULL;
    __db__host_ = NULL;
    __db__port_ = 3306;
    __db__pwd__ = NULL;
    __db__user_ = NULL;
    __db__sock_ = NULL;
}

DbHelper::~DbHelper()
{

    if(handle != nullptr){

        mysql_close(handle);
    }
}
bool DbHelper::connect() throw (const char *)
{
      if(handle != NULL){
        throw "MYSQL_HANDLE_NOT_NULL!";
        //return ;
      }
      handle = mysql_init(NULL);
      mysql_real_connect(handle,this->__db__host_,this->__db__user_,this->__db__pwd__,
                                   this->__db__dfl__dbn_,this->__db__port_,this->__db__sock_,0) ;
         if(this->get_errno()){

            throw this->get_errmsg();

         }
         mysql_query(handle,"set names utf8");

    return true;
}

bool DbHelper::setdb(const char* __dbname) noexcept
{
    if(handle == NULL){
        return false;
    }

    return mysql_select_db(handle,__dbname) == 0 ? true:false;
}

bool DbHelper::connect(const char* __db__host_,
                    unsigned int   __db__port_,
                       const char* __db__user_,
                       const char* __db__pwd__,
                       const char* __db__dfl__dbn_) throw (const char *)
{
      if(handle != NULL){
        throw "MYSQL_HANDLE_NOT_NULL!";
        //return ;
      }
      handle = mysql_init(NULL);
      mysql_real_connect(handle,__db__host_,
                            __db__user_,
                            __db__pwd__,
                            __db__dfl__dbn_,__db__port_,__db__sock_,0) ;

        if(this->get_errno()){

            throw this->get_errmsg();

         }
         mysql_query(handle,"set names utf8");
        return true;
}

unsigned int DbHelper::get_errno() const noexcept
{
     return mysql_errno(handle);
}

const char* DbHelper::get_errmsg() const noexcept
{
     return mysql_error(handle);
}

void DbHelper::set_sock(const char* sock) noexcept
{
    this->__db__sock_ = const_cast<char *>(sock);
}

unsigned int DbHelper::exec(const char* __cmd_query) throw(const char *)
{
    if(handle == nullptr){
        throw "db connection is not initialized";
    }
    if(!mysql_real_query(handle,__cmd_query,strlen(__cmd_query)))
        return mysql_affected_rows(handle);
    else
        return 0;
}
vector<vector<std::string>> DbHelper::query(const char* __cmd_query) throw (const char *)
{
    if(handle == nullptr){
        throw "db connection is not initialized";
    }
    vector< vector <std::string> > myVec;
    if(!mysql_real_query(handle,__cmd_query,strlen(__cmd_query))){
        result = mysql_use_result(handle);
        while((rows = mysql_fetch_row(result))){
            unsigned int __size = mysql_field_count(handle);
            vector<std::string> vecTmp;
            for(unsigned int  i = 0; i<__size ; ++i){
                // TODO (hafrans#1#): bugfix ...
                string __str_cxx_tmp(rows[i] == NULL ? "NULL" : rows[i]);

                vecTmp.push_back(__str_cxx_tmp);
            }
            myVec.push_back(vecTmp);
        }
        mysql_free_result(result);
    }else{
        throw this->get_errmsg();
    }

    return myVec;
}


vector<std :: string> DbHelper::queryOneRow(const char* __cmd_query) throw (const char *)
{
    if(handle == nullptr){
        throw "db connection is not initialized";
    }
    vector<std::string> vecTmp;
    if(!mysql_real_query(handle,__cmd_query,strlen(__cmd_query))){
        result = mysql_use_result(handle);
            if((rows = mysql_fetch_row(result))) {
                unsigned int __size = mysql_field_count(handle);
                for(unsigned int  i = 0; i<__size ; ++i){
                    // TODO (hafrans#1#): bugfix ...
                    std :: string __str_cxx_tmp(rows[i] == NULL ? "NULL" : rows[i]);

                    vecTmp.push_back(__str_cxx_tmp);
                }
            }
        mysql_free_result(result);
    }else{
        throw this->get_errmsg();
    }

    return vecTmp;
}



unsigned long DbHelper::escape(char* __str__dest, const char* __str_src) noexcept
{
    unsigned long __m_size = mysql_escape_string(__str__dest,__str_src,strlen(__str_src));
    __str__dest[__m_size] = 0;
    return __m_size;
}





