// Screenshot.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Screenshot.h"

#include <iostream>
#include <atlimage.h>
#include <comdef.h>
#include <time.h>
#include <chrono>
#include <thread>

// Not used for now. It is destined for compress_image() function, so that user can choose what extension they want.
enum class image_type
{
	JPEG,
	PNG
};

Screenshot::Screenshot()
{
}

void Screenshot::convert_string_to_wchar()
{
	std::copy(file_name.begin(), file_name.end(), file_buffer);
	file_buffer[file_name.size()] = '\0';
}

void Screenshot::compress_image()
{
	HRESULT result = 0;
	_com_error error_text = result;

	ATL::CImage img;
	result = img.Load(file_buffer);

	LPCTSTR error_message = error_text.ErrorMessage();
	//wprintf(L"Result [load]: %s\n", error_message);

	result = img.Save(file_buffer, Gdiplus::ImageFormatJPEG);
	//wprintf(L"Result [save]: %s\n", error_message);
}

void Screenshot::save_screenshot_as_image(HBITMAP bitmap, HDC hDC, std::string optional_file_name = "")
{
	BITMAP bmp;
	PBITMAPINFO pbmi;
	WORD cClrBits;
	HANDLE hf; // file handle 
	BITMAPFILEHEADER hdr; // bitmap file-header 
	PBITMAPINFOHEADER pbih; // bitmap info-header 
	LPBYTE lpBits; // memory pointer 
	DWORD dwTotal; // total count of bytes 
	DWORD cb; // incremental count of bytes 
	BYTE *hp; // byte pointer 
	DWORD dwTmp;

	// A file name will be a full date.
	time_t current_time;
	char time_buffer[64];
	
	struct tm time_info;
	time(&current_time);
	localtime_s(&time_info ,&current_time);

	strftime(time_buffer, sizeof(time_buffer), "%d-%m-%G__%H_%M_%S", &time_info);
	file_name = time_buffer;
	file_name.append(optional_file_name);
	file_name.append(".bmp");
	convert_string_to_wchar();

	if (!GetObject(bitmap, sizeof(BITMAP), (LPSTR)&bmp))
	{
		printf("Could not retrieve bitmap info.");
		return;
	}

	// Convert the color format to a count of bits. 
	cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);
	if (cClrBits == 1)
		cClrBits = 1;
	else if (cClrBits <= 4)
		cClrBits = 4;
	else if (cClrBits <= 8)
		cClrBits = 8;
	else if (cClrBits <= 16)
		cClrBits = 16;
	else if (cClrBits <= 24)
		cClrBits = 24;
	else cClrBits = 32;
	// Allocate memory for the BITMAPINFO structure.
	if (cClrBits != 24)
		pbmi = (PBITMAPINFO)LocalAlloc(LPTR,
			sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1 << cClrBits));
	else
		pbmi = (PBITMAPINFO)LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER));

	// Initialize the fields in the BITMAPINFO structure. 

	pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pbmi->bmiHeader.biWidth = bmp.bmWidth;
	pbmi->bmiHeader.biHeight = bmp.bmHeight;
	pbmi->bmiHeader.biPlanes = bmp.bmPlanes;
	pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;
	if (cClrBits < 24)
		pbmi->bmiHeader.biClrUsed = (1 << cClrBits);

	// If the bitmap is not compressed, set the BI_RGB flag. 
	pbmi->bmiHeader.biCompression = BI_RGB;

	// Compute the number of bytes in the array of color 
	// indices and store the result in biSizeImage. 
	pbmi->bmiHeader.biSizeImage = (pbmi->bmiHeader.biWidth + 7) / 8 * pbmi->bmiHeader.biHeight * cClrBits;
	// Set biClrImportant to 0, indicating that all of the 
	// device colors are important. 
	pbmi->bmiHeader.biClrImportant = 0;

	// now open file and save the data
	pbih = (PBITMAPINFOHEADER)pbmi;
	lpBits = (LPBYTE)GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);

	if (!lpBits)
	{
		printf("writeBMP::Could not allocate memory");
		return;
	}

	// Retrieve the color table (RGBQUAD array) and the bits 
	if (!GetDIBits(hDC, HBITMAP(bitmap), 0, (WORD)pbih->biHeight, lpBits, pbmi,
		DIB_RGB_COLORS))
	{
		printf("writeBMP::GetDIB error");
		return;
	}

	// Create the .BMP file. 
	hf = CreateFile(file_buffer, GENERIC_READ | GENERIC_WRITE, (DWORD)0,
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,
		(HANDLE)NULL);
	if (hf == INVALID_HANDLE_VALUE)
	{
		printf("Could not create file for writing");
		return;
	}
	hdr.bfType = 0x4d42; // 0x42 = "B" 0x4d = "M" 
							// Compute the size of the entire file. 
	hdr.bfSize = (DWORD)(sizeof(BITMAPFILEHEADER) +
		pbih->biSize + pbih->biClrUsed
		* sizeof(RGBQUAD) + pbih->biSizeImage);
	hdr.bfReserved1 = 0;
	hdr.bfReserved2 = 0;

	// Compute the offset to the array of color indices. 
	hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) +
		pbih->biSize + pbih->biClrUsed
		* sizeof(RGBQUAD);

	// Copy the BITMAPFILEHEADER into the .BMP file. 
	if (!WriteFile(hf, (LPVOID)&hdr, sizeof(BITMAPFILEHEADER),
		(LPDWORD)&dwTmp, NULL))
	{
		printf("Could not write in to file: Copy the BITMAPFILEHEADER into the .BMP file failed.");
		return;
	}

	// Copy the BITMAPINFOHEADER and RGBQUAD array into the file. 
	if (!WriteFile(hf, (LPVOID)pbih, sizeof(BITMAPINFOHEADER)
		+ pbih->biClrUsed * sizeof(RGBQUAD),
		(LPDWORD)&dwTmp, (NULL)))
	{
		printf("Could not write in to file: Copy the BITMAPINFOHEADER and RGBQUAD array into the file failed.");
		return;
	}


	// Copy the array of color indices into the .BMP file. 
	dwTotal = cb = pbih->biSizeImage;
	hp = lpBits;
	if (!WriteFile(hf, (LPSTR)hp, (int)cb, (LPDWORD)&dwTmp, NULL))
	{
		printf("Could not write in to file: Copy the array of color indices into the .BMP file.");
		return;
	}

	// Close the .BMP file. 
	if (!CloseHandle(hf))
	{
		printf("Could not close file");
		return;
	}

	printf("Image saved.\n");

	// Free memory. 
	GlobalFree((HGLOBAL)lpBits);
}

/* Takes a screenshot as a bitmap and saves it to the disk */
void Screenshot::take_screenshot(std::string optional_file_name = "")
{
	int x1, y1, x2, y2, width, height;

	// get screen dimensions
	x1 = GetSystemMetrics(SM_XVIRTUALSCREEN);
	y1 = GetSystemMetrics(SM_YVIRTUALSCREEN);
	x2 = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	y2 = GetSystemMetrics(SM_CYVIRTUALSCREEN);
	width = x2 - x1;
	height = y2 - y1;

	// copy screen to bitmap
	HDC     hScreen = GetDC(NULL);
	HDC     hDC = CreateCompatibleDC(hScreen);
	HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, width, height);
	HGDIOBJ old_obj = SelectObject(hDC, hBitmap);
	BOOL    bRet = BitBlt(hDC, 0, 0, width, height, hScreen, x1, y1, SRCCOPY);

	// save bitmap to clipboard
	OpenClipboard(NULL);
	EmptyClipboard();
	SetClipboardData(CF_BITMAP, hBitmap);
	CloseClipboard();

	save_screenshot_as_image(hBitmap, hDC, optional_file_name);

	// clean up
	SelectObject(hDC, old_obj);
	DeleteDC(hDC);
	ReleaseDC(NULL, hScreen);
	DeleteObject(hBitmap);
}

void Screenshot::auto_start()
{
	while (1)
	{
		take_screenshot();
		compress_image();
		std::chrono::seconds sleep_duration(screenshot_timer);
		std::this_thread::sleep_for(sleep_duration);
	}
}