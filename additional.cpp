//Дополнительные функции - строковые, работа с регулярками, список файлов в текущей папке(удоволетворяющих условию tasks-*.txt)
#include "additional.h"

vector<string> split(const string& s, char delimiter)
{
   vector<string> tokens;
   string token;
   istringstream tokenStream(s);
   while (getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}


vector<string> tasksFiles()
{
	vector<string> result;
	DIR *dir;
	char cwd[PATH_MAX];
	getcwd(cwd, sizeof(cwd));
	struct dirent *ent;
	if ((dir = opendir (cwd)) != NULL) {
	  /* print all the files and directories within directory */
	  while ((ent = readdir (dir)) != NULL) {
		  string f(ent->d_name);
		  if(f.find("task-") != std::string::npos)
			result.push_back(f);
	  }
	  closedir (dir);
	} else {
	}

	return result;
}

/*Названия функций поиска с использованием регулярным выражениям сделаны по аналогии с существующими в других языках */
vector<string> preg_match_all(const std::string & rex,const string & str,unsigned int id)
{
	vector<string> result;
	std::regex re(rex);
	std::smatch match;
	string::const_iterator searchStart( str.cbegin() );
	while(std::regex_search(searchStart, str.cend(), match, re)) {
		if(id < match.size()) result.push_back(match[id]);
		searchStart = match.suffix().first;
	}

	return result;
}


string preg_match(const std::string & rex,const string & str,unsigned int id)
{
	string result = "";
	std::regex re(rex);
	std::smatch match;
	if(std::regex_search(str, match, re)) {
		if(id < match.size()) result = match[id];
	}

	return result;
}

