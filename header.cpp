#ifndef _MDLP_HEADER
#define _MDLP_HEADER

#include <conio.h>
#include <string>
#include <io.h>
#include <iostream>
#include <windows.h>
using namespace std;

const int OneSecond=1000;

char lower(char c)
{
    return (c>=65 and c<=90) ? c+32 : c;
}
void Print(string s,int times)
{
    for(auto v:s)
    {
        cout<<v;
        if(_kbhit())
        {
            char c=_getch();
            times = INT_MAX;
        }
        Sleep(OneSecond/times);
    }

}
char WaitForInput()
{
    char ch = 0;
    while(true)
    {
        if(!_kbhit()) continue;
        else { ch = _getch();break; }
    }
    return ch;
}

#endif // _MDLP_HEADER
