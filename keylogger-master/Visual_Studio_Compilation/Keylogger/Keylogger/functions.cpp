#include "stdafx.h"

#include "functions.h"

using namespace std;

std::string intToString(int i){
	char buffer[4];
	_itoa_s(i, buffer, 4, 10);
	return std::string(buffer);
}

std::string getCurrDir(){
	char *curdir = new char[MAX_PATH];
	wchar_t *curdir_wchar = new wchar_t[MAX_PATH];
	
	GetCurrentDirectory(MAX_PATH, curdir_wchar);
	size_t pReturnValue;
	wcstombs_s(&pReturnValue, curdir, MAX_PATH, curdir_wchar, MAX_PATH);
	std::string rv(curdir);
	delete[] curdir;
	return rv;
}

std::string getSelfPath(){
	char selfpath[MAX_PATH];
	wchar_t selfpath_wchar[MAX_PATH];
	
	GetModuleFileName(NULL, selfpath_wchar, MAX_PATH);
	size_t pReturnValue;
	wcstombs_s(&pReturnValue, selfpath, MAX_PATH, selfpath_wchar, MAX_PATH);

	return std::string(selfpath);
}

string dirBasename(string path){
	if(path.empty())
		return string("");
	
	if(path.find("\\") == string::npos)
		return path;
	
	if(path.substr(path.length() - 1) == "\\")
		path = path.substr(0, path.length() - 1);
	
	size_t pos = path.find_last_of("\\");
	if(pos != string::npos)
		path = path.substr(0, pos);
	
	if(path.substr(path.length() - 1) == "\\")
		path = path.substr(0, path.length() - 1);
	
	return path;
}
