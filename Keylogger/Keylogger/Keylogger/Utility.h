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

	void create_folder(const wchar_t * path);

	BOOL IsMyProgramRegisteredForStartup(PCWSTR pszAppName);
	BOOL RegisterMyProgramForStartup(PCWSTR pszAppName, PCWSTR pathToExe, PCWSTR args);
	void RegisterProgram();
};

// Related to configuration.txt file that has user configuration
namespace configuration
{
	const std::string logs_base_path = "C:\\Users\\Public\\Documents\\service";
	const std::string configuration_file_name = "C:\\Users\\Public\\Documents\\service\\configuration.txt";
	const std::wstring application_name = L"Windows health service";
	
	struct configuration_file_content
	{
		std::string mail_to;
		int mail_send_loop;
		std::vector<std::string> keywords;
	};

	configuration_file_content load_configuration_from_file();

	void save_configuration_to_file(std::string mail_to, int email_send_loop_time, std::vector<std::string> keywords);
};