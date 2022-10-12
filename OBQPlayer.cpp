/*
The player of the OBQ
Author: QingChenling
License: GNU General Public License
Source: http://www.github.com/LordLaffey/OBQ/
Language: cpp
LastEditor: 2022/10/12
version: v0.01
*/
#include <bits/stdc++.h>
#include <windows.h>
#include <conio.h>
using namespace std;

struct note
{
    int t,l;
};

int n;
note a[100100];
atomic_int cnt;
int start;
atomic_int pre,bad,miss;

// Clears the screen
void clrscr()
{
	COORD coordScreen = { 0, 0 };
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD dwConSize;
	HANDLE hConsole = GetStdHandle( STD_OUTPUT_HANDLE );

	GetConsoleScreenBufferInfo( hConsole, &csbi );
	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
	FillConsoleOutputCharacter( hConsole, TEXT(' '), dwConSize, coordScreen, &cCharsWritten );
	GetConsoleScreenBufferInfo( hConsole, &csbi );
	FillConsoleOutputAttribute( hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten );
	SetConsoleCursorPosition( hConsole, coordScreen );
}
void Layout()
{
    while(cnt<=n)
    {
        clrscr();
        if(a[cnt].t-(clock()-start) < -10) ++cnt,++miss;
        printf("Prefect: %d\t Bad: %d\t Miss: %d\n", (int)pre, (int)bad, (int)miss);
        puts("----------------------------------------------------------------");
        int nowt=clock();
        string str="";
        for(int i=1,j=cnt; i<=25; i++)
        {
            char s[10]=" \t \t \t ";
            if(a[j].t-(nowt-start)-i*50<200)
            {
                s[a[j].l*2-1]='*';
                ++j;
            }
            str=str+s+'\n';
        }
        cout<<str;
        this_thread::sleep_for(chrono::milliseconds(50));
    }
}
void UserListen()
{
    while(cnt<=n)
    {
        if(!_kbhit()) continue;
        int ch=_getch();
        int li=0;
        switch(ch)
        {
        case 100: //d
            li=1; break;
        case 102: //f
            li=2; break;
        case 106: //g
            li=3; break;
        case 107: //k
            li=4; break;
        }
        if(li)
        {
            if(li!=a[cnt].l) continue;
            if(abs(clock()-start-a[cnt].t)>1000) continue;
            if(abs(clock()-start-a[cnt].t)>300)
            ++bad;
            else
            ++pre;
            ++cnt;
        }
    }
    this_thread::sleep_for(chrono::seconds(1));
    clrscr();
    printf("Prefect: %d\n", (int)pre);
    printf("Bad: %d\n", (int)bad);
    printf("Miss: %d\n", (int)miss);
}
int main()
{
    puts("Anykey to start replay"); _getch();
    // puts("input the music file(def:wyyyy.in):");

    string music_file="record.rbq";
    // getline(music_file, cin);
    FILE* file=fopen(music_file.data(),"r");
    fscanf(file, "%d", &n);
    for(int i=1; i<=n; i++)
    fscanf(file, "%d%d", &a[i].t, &a[i].l);
    fclose(file);

    start=clock();
    thread layout(Layout),listen(UserListen);
    layout.join(); listen.join();
    return 0;
}