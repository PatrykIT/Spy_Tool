#pragma once
#include <Windows.h>
#include <string>

class Screenshot
{
private:
	std::string file_name;
	wchar_t file_buffer[64];
	int screenshot_timer = 30;

	void save_screenshot_as_image(HBITMAP bitmap, HDC hDC, std::string optional_file_name);
	void convert_string_to_wchar();

public:
	Screenshot();
	void take_screenshot(std::string optional_file_name);
	void compress_image();
	void auto_start();
};