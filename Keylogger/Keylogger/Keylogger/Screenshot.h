#pragma once
#include <Windows.h>
#include <string>

class Screenshot
{
private:
	std::string file_name;
	wchar_t file_buffer[64];
	
	void save_image(HBITMAP bitmap, HDC hDC);
	void convert_string_to_wchar();

public:
	Screenshot();
	void GetScreenShot();
	void compress_image();
};