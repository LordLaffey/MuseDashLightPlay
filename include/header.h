/**
 * @details header
*/
#ifndef _MDLP_HEADER
#define _MDLP_HEADER

#include <atomic>
#include <cassert>
#include <conio.h>
#include <cstring>
#include <iostream>
#include <io.h>
#include <map>
#include <set>
#include <string>
#include <thread>
#include <vector>
#include <windows.h>
#include <winspool.h>

static const int OneSecond = 1000;
static const int MDFallTime = 1500;
static const int FourKeyFallTime = 1000;
static const int Columns = 45;
static const int Lines = 15;
static const double MDSpeed = (double)Columns / MDFallTime;
static const double FourKeySpeed = (double)Lines / FourKeyFallTime;

static int start_time;
static int NowTime()
{
    return clock()-start_time;
}

static char lower(char c)
{
    return (c>=65 and c<=90) ? c+32 : c;
}

static void Print(std::string s, int times)
{
    for(auto v : s)
    {
        std::cout << v;
        if(_kbhit()) times = INT_MAX;
        Sleep(OneSecond/times);
    }
}

static char WaitForInput()
{
    char ch = 0;
    while(true)
    {
        if(!_kbhit()) continue;
        else { ch = _getch(); break; }
    }
    return ch;
}

static void ClearScreen()
{
	COORD coordScreen = {0, 0};
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD dwConSize;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);    
    
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
	FillConsoleOutputCharacter(hConsole, TEXT(' '), dwConSize, coordScreen, &cCharsWritten);
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	SetConsoleCursorPosition(hConsole, coordScreen);
}

static void CheckFiles(){
    
    if(access("data",0) == -1)
        mkdir("data");
    if(access("data/music",0) == -1)
        mkdir("data/music");
    if(access("data/music/MuseDashMode",0) == -1)
        mkdir("data/music/MuseDashMode");
    if(access("data/music/4KMode",0) == -1)
        mkdir("data/music/4KMode");
    if(access("include/header.h",0)==-1)
        puts("Error: 101");
    if(access("include/double_console.h",0)==-1)
        puts("Error: 102");
    
}

#endif // _MPLP_HEADER
