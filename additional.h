//Дополнительные функции - строковые, работа с регулярками, список файлов в текущей папке(удоволетворяющих условию tasks-*.txt)
#include <iostream>
#include <string>
#include <curl/curl.h>
#include <regex>
#include <algorithm>
#include <fstream>
#include "dirent.h"
#include <unistd.h>
#include <thread>

using namespace std;
vector<string> split(const string& s, char delimiter);
vector<string> tasksFiles();
vector<string> preg_match_all(const std::string & rex,const string & str,unsigned int id);
string preg_match(const std::string & rex,const string & str,unsigned int id);
