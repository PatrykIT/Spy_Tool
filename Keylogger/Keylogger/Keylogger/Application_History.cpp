#include "stdafx.h"

#include "Application_History.h"
#include <windows.h>
#include <iostream>
#include <fstream>

#define DEBUG

Application_History::Application_History(std::ofstream &_applications_log_with_keys,
	std::ofstream &_applications_log_with_clean_keys, const std::string &file_path, bool save_enabled) :
	applications_log_with_keys(_applications_log_with_keys),
	applications_log_with_clean_keys(_applications_log_with_clean_keys)
{
	saving_enabled = save_enabled;
	
	// Append a string to the name before the extension
	size_t position_to_prepend = file_path.find(".log");
	std::string path_for_applications_only = file_path;
	path_for_applications_only.insert(position_to_prepend, "_applications_only");
	applications_log.open(path_for_applications_only);
}

void Application_History::save_to_file(std::ofstream &file_stream, const std::string &content)
{
	if (saving_enabled)
	{
		file_stream << content;
		file_stream.flush();
	}
}


std::string Application_History::get_window_name()
{
	// get the active windowtitle
	char title[1024];
	wchar_t title_wchar[1024];
	HWND hwndHandle = GetForegroundWindow();
	GetWindowText(hwndHandle, title_wchar, 1023);

	size_t pReturnValue;
	wcstombs_s(&pReturnValue, title, MAX_PATH, title_wchar, MAX_PATH);

	current_title = std::string(title);
	return current_title;
}

void Application_History::update_window()
{
	std::string PL_window = "APLIKACJA: ";
	std::string PL_no_active_window = "NIE MA AKTYWNEJ APLIKACJI";
	if (last_title != current_title)
	{
		std::string content;
		
		content = "\n\n" + PL_window;
		save_to_file(applications_log, content);
		save_to_file(applications_log_with_keys, content);
		save_to_file(applications_log_with_clean_keys, content);

#ifdef DEBUG
		std::cout << content;
#endif
		if (current_title.empty())
		{
			content = PL_no_active_window;
			save_to_file(applications_log, content);
			save_to_file(applications_log_with_keys, content);
			save_to_file(applications_log_with_clean_keys, content);
#ifdef DEBUG
			std::cout << content;
#endif
		}
		else
		{
			content = "'" + current_title + "'";
			save_to_file(applications_log, content);
			save_to_file(applications_log_with_keys, content);
			save_to_file(applications_log_with_clean_keys, content);
#ifdef DEBUG
			std::cout << content;
#endif
		}
		content = "\n";
		save_to_file(applications_log, content);
		save_to_file(applications_log_with_keys, content);
		save_to_file(applications_log_with_clean_keys, content);
#ifdef DEBUG
		std::cout << content;
#endif

		last_title = current_title;
	}
}