/*Helpful Forum Poster's Console Maker Code Starts Here*/
#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>

static void Redirect(FILE *pFile, const char *sMode, HANDLE hHandle)
{
    int nFileHandle = _open_osfhandle((long)hHandle, _O_TEXT);
    FILE *fp = _fdopen(nFileHandle, sMode);
 
    *pFile = *fp;
 
    setvbuf(pFile, NULL, _IONBF, 0);
}
 
void CreateConsole(void)
{
    AllocConsole();
 
    CONSOLE_SCREEN_BUFFER_INFO oConsoleInfo;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &oConsoleInfo);
 
    /// Change console buffer size
    oConsoleInfo.dwSize.X = 80;   /// Characters per line
    oConsoleInfo.dwSize.Y = 5000; /// Number of lines
    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), oConsoleInfo.dwSize);
 
    HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE hInput  = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE hError  = GetStdHandle(STD_ERROR_HANDLE);
 
    /// Redirect STDOUT, STDIN and STDERR to our console window
    Redirect(stdout, "w", hOutput);
    Redirect(stdin,  "r", hInput);
    Redirect(stderr, "w", hError);
 
    /// Redirect cout, cin, cerr, clog, etc. to our console window
    std::ios::sync_with_stdio();
}
 
void RemoveConsole(void)
{
    FreeConsole();
}
/*End***********************************/