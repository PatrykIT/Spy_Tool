#pragma once

#include <string>
#include <fstream>

class Application_History
{
	std::string last_title = "";
	std::string current_title = "";
	std::ofstream applications_log;
	std::ofstream &applications_log_with_keys;
public:
	Application_History(std::ofstream &_applications_log_with_keys, const std::string &file_path);
	
	std::string get_window_name();
	void update_window();
};
