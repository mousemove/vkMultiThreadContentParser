
#include "Task.h"
//curl functions start//
size_t writeFunction(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	size_t written;
	written = fwrite(ptr, size, nmemb, stream);
	return written;
}


size_t readFunction(char *contents, size_t size, size_t nmemb, void *userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}
//curl functions end//

void Task::save()
{
	ofstream taskf(_name);
	if(taskf.is_open())
	{
		taskf << _dir << ";" << _filename << ";" << to_string(_offset) << ";" << to_string(_needed) << ";" << _id << ";" << to_string((unsigned)_type) << ";" << to_string(_parsed) << endl;
	}

}

void Task::operator()(){
	while(contentFinish == false)
	{
		cout << "STATUS::  " << _name << " parsing:" << _parsed <<" / " << _needed << "  "<< string("https://m.vk.com/"+_id+"?offset="+to_string(_offset)+"&own=1") << endl;
		int parsed = (_type == Images ? parseImage(string("https://m.vk.com/"+_id+"?offset="+to_string(_offset)+"&own=1").c_str()) : parseText(string("https://m.vk.com/"+_id+"?offset="+to_string(_offset)+"&own=1").c_str()));
		_parsed += parsed;
		if(_needed != -1 && _parsed  >= _needed) contentFinish = true;
		save();
		std::this_thread::sleep_for(std::chrono::microseconds(_timeout));
	}
	remove(_name.c_str());
	cout << "PARSING FINISHED::  " << _name << endl;
}



bool Task::loadAndSaveFileCurl(const char *url, const char * path)
{
	CURL *curl;
	FILE *fp;
	curl = curl_easy_init();
	if (curl)
	{
		fp = fopen(path,"ab+");
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Linux; Android 9; SM-G960F Build/PPR1.180610.011; wv) AppleWebKit/537.36 (KHTML, like Gecko) Version/4.0 Chrome/74.0.3729.157 Mobile Safari/537.36");
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		fclose(fp);
		return 1;
	}
	return 0;
}

string Task::HTTPgetCURL(const char *url)
{
	CURL *curl;
	string result = "";
	curl = curl_easy_init();
	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, readFunction);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Linux; Android 9; SM-G960F Build/PPR1.180610.011; wv) AppleWebKit/537.36 (KHTML, like Gecko) Version/4.0 Chrome/74.0.3729.157 Mobile Safari/537.36");
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
		curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		return result;
	}
	return "";
}

int Task::parseText(const char * l)
{
	string source = HTTPgetCURL(l);
	if (source.size() == 0) return 0;
	auto wallposts = preg_match_all("<div class=\"wall_item\">((.|\n)*?)<div class=\"wi_buttons\">",source,1);
	if(wallposts.size() == 0) {contentFinish = true;return 0;}
	int count = 0;
	_offset += _offset > 10 ? wallposts.size() + 1 :  wallposts.size();
	for(auto post: wallposts)
	{
		if(post.find("pic_from") != std::string::npos) continue;//репощенное не парсим
		auto text = preg_match("<div class=\"pi_text\">((.|\n)*?)</div>",post,1);
		if(checkStopWords(text) && text.size() > _minLengthTXT) {
			text = std::move(std::regex_replace(text,std::regex("<[^>]*>"),""));
			//cout << count++ << " "  << text << " " << _filename << endl;
			ofstream out(_dir != "" ?_dir+"/"+_filename : _filename,ios_base::app);
			if(out.is_open())
			{
				count++;
				out << text << endl;
				out.close();
			}
		}
	}
	return count;
}


int Task::parseImage(const char * l)
{
	string source = HTTPgetCURL(l);
	if (source.size() == 0) return 0;
	auto wallposts = preg_match_all("<div class=\"wall_item\">((.|\n)*?)<div class=\"wi_buttons\">",source,1);
	if(wallposts.size() == 0) {cout<< "Content finish" << endl;contentFinish = true;return 0;}
	int count = 0;
	_offset += _offset > 10 ?  wallposts.size() + 1 :  wallposts.size();
	for(auto post: wallposts)
	{
		if(post.find("pic_from") != std::string::npos) continue;//репощенное не парсим
		auto text = preg_match("<div class=\"pi_text\">((.|\n)*?)</div>",post,1);
		if(checkStopWords(text))
		{
			auto picURL = preg_match("data-src_big=\"((.|\n)*?)[|\"]",post,1);
			if(picURL != "")
			{
				loadAndSaveFileCurl(picURL.c_str(), string((_dir != "" ?(_dir+"/") : "")+preg_match("/([A-Za-z0-9-_]*.jpg)",picURL,1)).c_str());

				count++;
			}
		}
	}
	return count;
}

bool Task::checkStopWords(string s)
{
	for(auto sw: stopWords)
	{
		if(s.find(sw) != std::string::npos) return false;
	}
	return true;
}

void Task::setStopWords(vector<string> & sw)
{

	stopWords = sw;

}
