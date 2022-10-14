/**
 * @file xkey_player.cpp
 * @authors LordLaffey, Ptilopsis_w
 * @version v0.1
 * @date 2022-10-13
 */

#include "header.cpp"
#include "settings.cpp"
#include "Music.cpp"
using namespace std;
using namespace song;


void FourKeyPlayerMain();
void FourkeyPrintScreen();

template<int id>
void XkeyCheckKeys();

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
    
    for(int i = 1; i <= 4; i++)
        track[i].init(i);
    song::reset();
    
    cout << "Press any key to start" << endl;
    WaitForInput();
    Print("Ready...\n", 9);
    Print("GO!!!\n", 6);
    
    start_time = clock();
    thread print(FourkeyPrintScreen);
    thread check1(XkeyCheckKeys<1>);
    thread check2(XkeyCheckKeys<2>);
    thread check3(XkeyCheckKeys<3>);
    thread check4(XkeyCheckKeys<4>);
    print.join(); 
    check1.join(); 
    check2.join();
    check3.join();
    check4.join();
    
    ClearScreen();
    cout << "----------------------------" << endl;
    cout << "Ended" <<endl;
    cout << "Press any key return to the main menu" << endl;
    WaitForInput();
    ClearScreen();
}

template<int id>
void XkeyCheckKeys()
{
    Track &t = track[id];
    while(t.now_note <= t.note_cnt)
    {
        while(t.now_note <= t.note_cnt and !GetNoteState(t.note[t.now_note]))
            miss_tot++, t.now_note++, song::now_note++;
        while(t.can_seen <= t.note_cnt and GetNoteState(t.note[t.can_seen]) != 5)
            t.can_seen++;
        if(GetNoteState(t.note[t.now_note]) >= 4) continue;
        if(!_kbhit()) continue;
        char c = _getch();
        if(setting.checkKey(c) == id)
        {
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
}

void FourkeyPrintScreen()
{
    char output[20][40];
    char buf[1000];
    memset(output,' ',sizeof(output));
    while(song::now_note <= song::note_cnt)
    {
        ClearScreen();
        cout << "Perfect\tGood\tBad\tMiss" << endl;
        cout << (int)perfect_tot << "\t" << (int)good_tot << "\t" 
             << (int)bad_tot << "\t" << (int)miss_tot << endl;
        cout << "----------------------------" << endl;
        memset(output,0,sizeof(output));
        for(int i=1;i<=4;i++)
        {
            const Track &t = track[i];
            for(int j = t.now_note; j < t.can_seen; j++)
            {
                int pos = 15-Xkey_Speed * (t.note[j] - NowTime());
                if(pos < 0 || pos > 15) continue;
                output[pos][(i-1)*8+1] = 'x';
                output[pos][(i-1)*8+2] = 'x';
                output[pos][(i-1)*8+3] = 'x';
                output[pos][(i-1)*8+4] = 'x';
            }
        }
        for(int i=1;i<=15;i++)
            for(int j=1;j<=30;j++)
                if(!output[i][j]) output[i][j] = ' ';
        
        memset(buf, 0, sizeof(buf));
        for(int i=1;i<=15;i++)
        {
            output[i][29]='\n';
            strcat(buf+1,output[i]+1);
        }
        cout << (buf+1);
        cout << "====----====----====----====\n" ;
        this_thread::sleep_for(chrono::milliseconds(20));
    }
    cout << "Perfect\t\tGood\t\tBad\t\tMiss" << endl;
    cout << (int)perfect_tot << "\t\t" << (int)good_tot << "\t\t" 
        << (int)bad_tot << "\t\t" << (int)miss_tot << endl;
}

/* UI

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
