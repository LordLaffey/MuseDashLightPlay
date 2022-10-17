/**
 * @file xkey_player.cpp
 * @authors LordLaffey, Ptilopsis_w
 * @version v1.04
 * @date 2022-10-17
 */

#include "include/header.h"
#include "include/console.h"
#include "include/song.h"
#include "settings.cpp"
#include "music.cpp"
using namespace std;

void FourKeyPlayerMain();
void FourkeyPrintScreen();
void XkeyCheckKeys();
void XkeyChangeStatus(int);
void XkeyPrework();
void XkeyEnd();

atomic<int> fourkey_combo;
atomic<int> fourkey_status,fourkey_status_start;
atomic<bool> fourkey_quit_flag;
void FourKeyPlayerMain()
{
    ClearScreen();
    
    XkeyPrework();
    
    cout << "Press any key to start" << endl;
    WaitForInput();
    Print("Ready...\n", 9);
    Print("GO!!!\n", 6);

    con.open();
    start_time = clock();
    thread print(FourkeyPrintScreen);
    thread check(XkeyCheckKeys);
    print.join();
    check.join();
    con.close();

    XkeyEnd();
    ClearScreen();

}

// to make the Main() function more clear
void XkeyPrework(){
    
    FILE* fr = Music.ChooseMusic(2);
    if(!song.LoadSpectrum(fr))
    {
        cout << "No such file!" << endl;
        Sleep(OneSecond);
        return void();
    }
    
    fourkey_combo = 0;
    fourkey_quit_flag = false;
    XkeyChangeStatus(-1);
    
}

// to make the Main() function more clear
void XkeyEnd(){
    
    ClearScreen();
    cout << "Perfect\tGood\tBad\tMiss" << endl;
    cout << (int)song.perfect_tot << "\t" << (int)song.good_tot << "\t" 
        << (int)song.bad_tot << "\t" << (int)song.miss_tot << endl;
    cout << "----------------------------" << endl;
    cout << "Ended" <<endl;
    cout << "Press 'q' to return to the main menu" << endl;
    
    while(1)
    {
        if(!_kbhit()) continue;
        char c = _getch();
        if(c == 'q') break;
    }
    
}

void XkeyCheckKeys()
{
    while(!song.isEnd())
    {
        int tmp=song.run();
        if(tmp) XkeyChangeStatus(0),fourkey_combo=0;
        song.miss_tot += tmp;
        if(!_kbhit()) continue;// @pt 你体验一下
        char c = _getch();
        if(c == 27) return fourkey_quit_flag=true,void();
        int now = setting.checkKey(c);
        if(now==-1) continue;
        switch(song.getStatus(now))
        {
            case 1: song.perfect_tot++; XkeyChangeStatus(1); fourkey_combo++; break;
            case 2: song.good_tot++; XkeyChangeStatus(2); fourkey_combo++; break;
            case 3: song.bad_tot++; XkeyChangeStatus(3); fourkey_combo = 0; break;
        }
    }
}

/**
 * 屏幕打印线程
*/
void FourkeyPrintScreen()
{
    static char output[20][40];
    static char buf[1000];
    static vector<int> note;
    memset(output, ' ', sizeof(output));
    while(!song.isEnd())
    {
        if(fourkey_quit_flag) return ;
        ClearScreen();
        con << "Perfect\tGood\tBad\tMiss" << endl;
        con << (int)song.perfect_tot << "\t" << (int)song.good_tot << "\t" 
            << (int)song.bad_tot << "\t" << (int)song.miss_tot << endl;
        con << "----------------------------" << endl;
        memset(output, 0, sizeof(output));
        for(int i = 1; i <= 4; i++)
        {
            note=song.getNotes(i);
            for(auto v:note)
            {
                int pos = 15-FourKeySpeed * (v - NowTime())+2;
                if(pos < 0 || pos > 15) continue;
                output[pos][(i-1)*8+1] = 'x';
                output[pos][(i-1)*8+2] = 'x';
                output[pos][(i-1)*8+3] = 'x';
                output[pos][(i-1)*8+4] = 'x';
            }
        }
        for(int i = 1; i <= 15; i++)
            for(int j = 1; j <= 30; j++)
                if(!output[i][j]) output[i][j] = ' ';
        
        memset(buf, 0, sizeof(buf));
        for(int i = 1; i <= 15; i++)
        {
            output[i][29] = '\n';
            strcat(buf+1, output[i]+1);
        }
        con << (buf+1);
        con << "====----====----====----====" << endl;
        if(fourkey_status != -1 && NowTime() - fourkey_status_start <= Status_Time)
        {
            if(fourkey_status == 1) con << "Perfect";
            else if(fourkey_status == 2) con << "Good   ";
            else if(fourkey_status == 3) con << "Bad    ";
        }
        else con << "       ";
        if(fourkey_combo >= 5) con << setw(20) << "Combo: " << fourkey_combo;
        con.update();
        this_thread::sleep_for(chrono::milliseconds(20));
    }
    Sleep(OneSecond);
    
}

/**
 * 更改实时响应状态
*/
void XkeyChangeStatus(int status){
    
    fourkey_status = status;
    fourkey_status_start = NowTime();
    
}

/*

Perfect Good    bad    miss
7777    7777    7777   7777
----------------------------
                xxxx
        xxxx
xxxx
        xxxx
                xxxx
                        xxxx
                xxxx
        xxxx
xxxx
        xxxx
                xxxx
                        xxxx
                xxxx
        xxxx
xxxx
====----====----====----====

*/
