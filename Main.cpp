#undef UNICODE
#include <windows.h>
#include <stdio.h>
#include "resource.h"
#include <inttypes.h>
#include <process.h>

#pragma warning (disable:4996)

#define DIV 1024

BOOL CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
HICON icon;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
	icon = LoadIconA(hInstance, MAKEINTRESOURCE(IDI_ICON2));
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAINDIALOG), NULL, MainWndProc);
	return 0;
}


void ShowInfo(HWND wnd) {

	//Get Computer name
	char buf[100];
	DWORD size = 100;
	if (!GetComputerName(buf, &size)) strcpy_s(buf, "error");
	SetDlgItemText(wnd, IDC_COMPNAME, buf);


	//Get OS Version
	OSVERSIONINFO ver;
	ver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&ver);
	sprintf_s(buf, "%d.%d (build %d) %s", ver.dwMajorVersion, ver.dwMinorVersion, ver.dwBuildNumber, ver.szCSDVersion);
	SetDlgItemText(wnd, IDC_VERSION, buf);


	//Get User name
	size = 100;
	if (!GetUserName(buf, &size)) strcpy_s(buf, "error");
	SetDlgItemText(wnd, IDC_USERNAME, buf);


	//Get information
	MEMORYSTATUSEX statex;

	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);


	//Get total  RAM
	sprintf_s(buf, "%lu", statex.ullTotalPhys / DIV/DIV);
	SetDlgItemText(wnd, IDC_TOTALRAM, buf);
	
	//Get available RAM
	sprintf_s(buf, "%lu",statex.ullAvailPhys / DIV / DIV);
	SetDlgItemText(wnd, IDC_FREERAM, buf);

	//Percent of memory in use
	sprintf_s(buf, "%ld%%", statex.dwMemoryLoad);
	SetDlgItemText(wnd, IDC_MEMORYINUSE, buf);
	
	
	//GetLocalTime
	SYSTEMTIME localTime;
	GetLocalTime(&localTime);
	sprintf_s(buf, "The system time is: %02d.%02d.%02d %02d:%02d:%02d\n", localTime.wDay, localTime.wMonth, localTime.wYear, localTime.wHour, localTime.wMinute, localTime.wSecond);
	SetDlgItemText(wnd, IDC_DATETIME, buf);


	//Get time since power on
	DWORD currentTime = GetTickCount64();

	int days = (currentTime / (60 * 60 * 24 * 1000));
	int hours = (currentTime / ((1000 * 60 * 60)) % 24);
	int minutes = (currentTime / ((1000 * 60)) % 60);
	int seconds  = (currentTime / 1000) % 60;
	
	sprintf_s(buf, "%d days %d hours %d min. %d sec.", days,hours,minutes,seconds);
	SetDlgItemText(wnd, IDC_TIMESINCE, buf);


}

BOOL CALLBACK MainWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	
	switch (Msg) {
	case WM_INITDIALOG:
		SetWindowTextA( hWnd,"Computer Information");
		SetTimer(hWnd, NULL, 250, NULL);
		SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)icon);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			DestroyWindow(hWnd);
			return TRUE;
		}
		return FALSE;
	case WM_TIMER:
		ShowInfo(hWnd);
		return true;
	case WM_DESTROY:
		PostQuitMessage(0);
		return TRUE;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		return TRUE;

	}
	return FALSE;
}

