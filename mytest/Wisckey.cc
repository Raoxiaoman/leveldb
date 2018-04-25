#include "lab2_common.h"
#include <fstream>
#include <algorithm> 
#include <fstream>
#include <vector>      
#include <ctime>       
#include <cstdlib>    

// Author: Abhishek Sharma
// Program: WiscKey Key Value Store
class Wisckey
{
public:
    Wisckey (string dir,string logfile){
        dir = dir;
        logfile = logfile;
        leveldb = open_leveldb(logfile);
    };
    virtual ~Wisckey ();
    string wisckey_get(string &key);
    bool wisckey_put(string &key,const char* value);
    void Wisckey_del(string &key);

private:
    string dir;
    DB * leveldb;
    string logfile;
};

Wisckey::~Wisckey(){
}

void Wisckey::Wisckey_del(string &key){
    cout << "Key: " << key << endl; 
    leveldb_del(leveldb,key);
}

bool Wisckey::wisckey_put(string &key,const char* value) {
    FILE *logfile = fopen("vlogfile","ab+");
    if(logfile == nullptr){
        cout << "openfile error" << endl;
        return false;
    }
    fseek(logfile, 0l, SEEK_END);
    long offset = ftell(logfile);
    long size = strlen(value);
    cout << "offset" << offset << endl;
    cout << "value_size:" << size << endl;
    std::string vlog_offset = std::to_string(offset);
    std::string vlog_size = std::to_string(size);
    std::stringstream vlog_address;
    //vlog_address:"vlog_offset&&vlog_size"
    vlog_address << vlog_offset << "&&" << vlog_size;
    std::string s = vlog_address.str();	
    fwrite (value,size,1,logfile);
    fclose(logfile);
    leveldb_set(leveldb,key,s);
    return true;

}

string Wisckey::wisckey_get(string &key) {
    FILE *logfile = fopen("vlogfile","rb+");
    cout << "\n\t\tGet Function\n\n";
    cout << "Key Received: " << key << endl;

    string offsetinfo;
    const bool found = leveldb_get(leveldb, key, offsetinfo);
    if (found) {
        cout << "Offset and Length: " << offsetinfo << endl;
    }
    else {
        cout << "Record:Not Found" << endl;
        return "";
    }
    std::string value_offset;
    std::string value_length;
    std::string s = offsetinfo;
    std::string delimiter = "&&";
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        value_offset = token;
        s.erase(0, pos + delimiter.length());
    }
    value_length = s;

    cout << "Value Offset: " << value_offset << endl;
    cout << "Value Length: " << value_length << endl;

    std::string::size_type sz;
    long offset = std::stol (value_offset,&sz);
    long length = std::stol (value_length,&sz);

    void* value_record =malloc(length);
    //cout << ftell(wk->logread) << endl;
    fseek(logfile,offset,SEEK_SET);
    cout << ftell(logfile) << endl;
    //rewind(wk->logfile);
    //cout << ftell(wk->logfile) << endl;	
    size_t n = fread(value_record,length,1,logfile);
    if(n<1){
        cout << "fread error" << endl;
        return "";
    }
    cout << "LogFile Value: " << (char*)value_record<< endl;
    string rvalue((char*)value_record);
    return  rvalue;
}



// For testing wisckey functionality 
void testing_function(Wisckey &wk,string &key, string &value) 
{
    /* Setting Value and Testing it */

    cout << "\n\n\t\tInput Received\n" << endl;
    cout << "Key: " << key << endl;
    cout << "Value: " << value << endl;
    wk.wisckey_put(key, value.c_str());
    string v = wk.wisckey_get(key);
    cout << "get_value:" << v << endl;

}
int main(int argc, char ** argv)
{
    Wisckey wk("wisckey_dir","logfile");
    string key("raohui");
    string value = "nihaoma,jiayou";
    testing_function( wk,key, value);
    key = "han";
    value = "raohui";
    testing_function( wk,key, value);
    key = "raohui" ;
    cout << wk.wisckey_get(key) << endl;
    destroy_leveldb("wisckey_test_dir");       
    remove("logfile");
    return 0;
}
