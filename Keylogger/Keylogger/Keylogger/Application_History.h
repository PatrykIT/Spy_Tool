#pragma once

#include <string>
#include <fstream>

class Application_History
{
private:
	std::string last_title = "";
	std::string current_title = "";
	std::ofstream applications_log;
	std::ofstream &applications_log_with_keys;
	std::ofstream &applications_log_with_clean_keys;

	void save_to_file(std::ofstream &file_stream, const std::string &content);


public:
	bool saving_enabled = false;

	Application_History(std::ofstream &_applications_log_with_keys, std::ofstream &_applications_log_with_clean_keys,
		const std::string &file_path, bool save_enabled);
	std::string get_window_name();
	void update_window();
};
