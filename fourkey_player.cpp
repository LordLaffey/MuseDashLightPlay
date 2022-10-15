/**
 * @file xkey_player.cpp
 * @authors LordLaffey, Ptilopsis_w
 * @version v0.2
 * @date 2022-10-15
 */

#include "include/header.h"
#include "include/console.h"
#include "settings.cpp"
#include "music.cpp"
using namespace std;
using namespace song;


void FourKeyPlayerMain();
void FourkeyPrintScreen();
void XkeyCheckKeys();

atomic<bool> fourkey_quit_flag;
void FourKeyPlayerMain()
{
    ClearScreen();
    
    FILE* fr = Music.ChooseMusic(2);
    if(!LoadSpectrum(fr))
    {
        cout << "No such file!" << endl;
        Sleep(OneSecond);
        return void();
    }
    
    fourkey_quit_flag = false;
    for(int i = 1; i <= 4; i++)
        track[i].init(i);
    song::reset();
    
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

    Sleep(OneSecond);
    ClearScreen();
    cout << "Perfect\tGood\tBad\tMiss" << endl;
    cout << (int)perfect_tot << "\t" << (int)good_tot << "\t" 
        << (int)bad_tot << "\t" << (int)miss_tot << endl;
    cout << "----------------------------" << endl;
    cout << "Ended" <<endl;
    cout << "Press 'q' to return to the main menu" << endl;
    
    while(1)
    {
        if(!_kbhit()) continue;
        char c = _getch();
        if(c == 'q') break;
    }
    
    ClearScreen();

}

void XkeyCheckKeys()
{
    while(song::now_note <= song::note_cnt)
    {
        for(int i=1;i<=4;i++)
        {
            Track &t=track[i];
            while(t.now_note <= t.note_cnt and !GetNoteState(t.note[t.now_note]))
                miss_tot++, t.now_note++, song::now_note++;
            while(t.can_seen <= t.note_cnt and GetNoteState(t.note[t.can_seen]) != 5)
                t.can_seen++;
        }
        if(!_kbhit()) continue;
        char c = _getch();
        if(c == 27) return fourkey_quit_flag=true,void();
        int now = setting.checkKey(c);
        Track &t=track[now];
        if(GetNoteState(t.note[t.now_note]) >= 4) continue;
        switch(GetNoteState(t.note[t.now_note]))
        {
            case 1: perfect_tot++; break;
            case 2: good_tot++; break;
            case 3: bad_tot++; break;
        }
        t.now_note++;
        song::now_note++;
    }
}

void FourkeyPrintScreen()
{
    static char output[20][40];
    static char buf[1000];
    memset(output, ' ', sizeof(output));
    Sleep(OneSecond);
    while(song::now_note <= song::note_cnt)
    {
        if(fourkey_quit_flag) return ;
        ClearScreen();
        con << "Perfect\tGood\tBad\tMiss" << endl;
        con << (int)perfect_tot << "\t" << (int)good_tot << "\t" 
             << (int)bad_tot << "\t" << (int)miss_tot << endl;
        con << "----------------------------" << endl;
        memset(output, 0, sizeof(output));
        for(int i = 1; i <= 4; i++)
        {
            const Track &t = track[i];
            for(int j = t.now_note; j < t.can_seen; j++)
            {
                int pos = 15-FourKeySpeed * (t.note[j] - NowTime());
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
        con.update();
        this_thread::sleep_for(chrono::milliseconds(20));
    }
    
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
