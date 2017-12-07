#include "stdafx.h"

#include "Application_History.h"
#include <windows.h>
#include <iostream>
#include <fstream>

#define DEBUG

Application_History::Application_History(std::ofstream &_applications_log_with_keys, const std::string &file_path) : 
	applications_log_with_keys(_applications_log_with_keys)
{
	size_t position_to_prepend = file_path.find(".log");
	std::string path_for_applications_only = file_path;
	path_for_applications_only.insert(position_to_prepend, "_applications_only");
	applications_log.open(path_for_applications_only);
}

std::string Application_History::get_window_name()
{
#ifdef DEBUG_TO_LOGS
	//printf("Geting title of the window.\n");
#endif

	// get the active windowtitle
	char title[1024];
	wchar_t title_wchar[1024];
	HWND hwndHandle = GetForegroundWindow();
	GetWindowText(hwndHandle, title_wchar, 1023);

	size_t pReturnValue;
	wcstombs_s(&pReturnValue, title, MAX_PATH, title_wchar, MAX_PATH);

#ifdef DEBUG_TO_LOGS
	// save window name
#endif

	current_title = std::string(title);
	return current_title;
}

void Application_History::update_window()
{
	std::string PL_window = "APLIKACJA: ";
	std::string PL_no_active_window = "NIE MA AKTYWNEJ APLIKACJI";
	if (last_title != current_title)
	{
		applications_log << std::endl << std::endl << PL_window;
		applications_log_with_keys << std::endl << std::endl << PL_window;
#ifdef DEBUG
		std::cout << std::endl << std::endl << PL_window;
#endif
		if (current_title.empty())
		{
			applications_log << PL_no_active_window;
			applications_log_with_keys << PL_no_active_window;
#ifdef DEBUG
			std::cout << PL_no_active_window;
#endif
		}
		else
		{
			applications_log << "'" << current_title << "'";
			applications_log_with_keys << "'" << current_title << "'";
#ifdef DEBUG
			std::cout << "'" << current_title << "'";
#endif
		}
		applications_log << std::endl;
		applications_log_with_keys << std::endl;
#ifdef DEBUG
		std::cout << std::endl;
#endif

		last_title = current_title;
	}
}