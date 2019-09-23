

#ifndef TASK_H_
#define TASK_H_
using namespace std;
#include "additional.h"
enum Type
{
	Images,
	Text
};

class Task
{
private:
	string _filename;//Файл куда идет сохранения
	string _dir = "";//Папка
	unsigned int _offset = 0;
	int _needed = -1;//Если -1 то парсим пока не остановится/не закончатся данные
	string _id;//id паблика
	Type _type;//Парсятся картинки или текст
	int _parsed = 0;//количество спарсенных новостей в данной таске
	string _name;//название файла
	unsigned int _timeout = 1000000;//таймаут в микросекундах
	unsigned int _minLengthTXT = 20;//минимальная длинна текстового поста
	vector<string> stopWords;//вектор СТОП СЛОВ
	bool contentFinish = false;//сигнал если закончился контент в паблике
public:
	Task(string dir, string filename,unsigned int offset,unsigned int needed,string id,Type type,unsigned int parsed, string name)
:_filename(filename),_dir(dir),_offset(offset),_needed(needed),_id(id),_type(type),_parsed(parsed),_name(name)
{

};
	void save();//Перезапись актуальной информации по таску
	void operator()();
	bool loadAndSaveFileCurl(const char *url, const char * path);//Функция сохранения изображения с помощью cURL
	string HTTPgetCURL(const char *url);// Загрузка текста с помощью cURL
	int parseImage(const char * l);
	int parseText(const char * l);
	bool checkStopWords(string s);//Проверка текста на стоп слова(спам, реклама и т.п.)
	void setStopWords(vector<string> & sw);//Выставка стоп слов
};

#endif /* TASK_H_ */
