/**
 * @details header
*/
#ifndef _MDLP_HEADER
#define _MDLP_HEADER

#include <iostream>
#include <cstdio>
#include <atomic>
#include <iomanip>
#include <vector>
#include <set>
#include <thread>
#include <windows.h>
#include <conio.h>
#include <io.h>

static const int OneSecond = 1000;
static const int MDFallTime = 1500;
static const int FourKeyFallTime = 1000;
static const int Columns = 45;
static const int Lines = 15;
static const double MDSpeed = (double)Columns / MDFallTime;
static const double FourKeySpeed = (double)Lines / FourKeyFallTime;
static const int Status_Time = 200;

static int start_time;
static int NowTime()
{
    return clock()-start_time;
}

std::pair<std::string,std::string> GetMinuteSecond(int time){
    
    time/=1000;
    int minute=time/60;
    int second=time%60;
    std::string min=std::to_string(minute),sec=std::to_string(second);
    if(minute<10) min="0"+min;
    if(second<10) sec="0"+sec;
    return std::make_pair(min,sec);
    
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
        if(_kbhit()) times = INT_MAX,_getch();
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

#endif // _MPLP_HEADER
