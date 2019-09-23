
#include <iostream>
#include "Task.h"
using namespace std;
void loader()
{
	//чтение новых тасков и создание для них отдельных файлов
	ifstream read("newtasks.txt");
	if (read.is_open())
	{
		string x ;
		while(getline(read,x)){
			auto params = split(x,';');
			if(params.size() == 6) {
				string d = x;
				d.erase(remove( d.begin(), d.end(), ';'),d.end());
				ofstream taskf("task-"+d+".txt");
				if(taskf.is_open())
				{
					taskf << x+ ";0"<< endl;
				}
			}
		}
		read.close();
		remove("newtasks.txt");//удаление файла после считывания - мы завели файлы на новые таски!
	}

	//Загрузка словаря стоп-слов
	vector<string> stopWords;
	ifstream readSW("stopwords.txt");
	if (readSW.is_open())
	{
		string sw;
		while(getline(readSW,sw)){
			stopWords.push_back(sw);
		}

	}


	//Создание потоков для парсинга данных и запуск
	vector<thread *> threads;
	auto taskFiles = tasksFiles();
	for(auto taskFile: taskFiles)
	{
		ifstream read(taskFile);
		if (read.is_open())
		{
			string x ;
			while(getline(read,x)){

				auto params = split(x,';');
				if(params.size() == 7) {
					Task * k  = new Task(params[0],params[1],stoul(params[2]),stoul(params[3]),params[4],(Type)stoul(params[5]),stoul(params[6]),taskFile);
					k->setStopWords(stopWords);
					thread * t = new thread(*k);
					threads.push_back(t);
				}

			}
		}

	}
	//Ждем завершение выполнения всех заданий по парсингу
	for(thread * t:threads)
	{
		t->join();
	}
}




int main() {
	loader();
	return 0;
}
