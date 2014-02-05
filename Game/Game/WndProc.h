#ifndef WINDOWS_H
	#define WINDDOWS_H
	#include <Windows.h>
#endif

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);               // Declaration For WndProc

extern bool    active;                                // Window Active Flag Set To TRUE By Default