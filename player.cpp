/**
 * @details The player of MDLP
 * @author Ptilosis_w, LordLaffey
 * @version v0.04
*/

#include "header.cpp"
#include "settings.cpp"

using namespace std;

const int FallTime = 1500;
const int Columns = 45;
const double Speed = (double)Columns / FallTime;

namespace song {
    const int Prefect = 80;
    const int Great = 200;
    int note_cnt;
    struct note {
        int time, line;
        int GetState();
    } note[100000];
    void reset();
}
using namespace song;


int start_time;
atomic<int> now_note, can_seen;
atomic<int> perfect_tot, great_tot, miss_tot;

bool LoadSpectrum();
char WaitForInput();
void PrintScreen();
void CheckKeys();
int NowTime();
void LoadSettings();
void PlayerMain();

void PlayerMain()
{
    ClearScreen();
    
    if(!LoadSpectrum()) return void();
    
    cout << "Press any key to start" << endl;
    
    WaitForInput();
    
    
    Print("Ready...\n", 9);
    Print("GO!!!\n", 6);
    
    song::reset();
    
    start_time = clock();
    thread print(PrintScreen);
    thread check(CheckKeys);
    print.join(); check.join();
    
    cout << "-----------------------------------------------";
    cout << "Ended" << endl;
    cout << "Press any key return to the main menu" << endl;
    WaitForInput();
    ClearScreen();
}

void song::reset()
{
    now_note = 0;
    can_seen = 0;
    perfect_tot = 0;
    great_tot = 0;
    miss_tot = 0;
}

void CheckKeys()
{
    now_note = 1;
    while(now_note <= note_cnt)
    {
        while(now_note <= note_cnt && !note[now_note].GetState())
            miss_tot++,now_note++;
        while(can_seen <= note_cnt && note[can_seen].GetState() != 4)
            can_seen++;
        if(_kbhit())
        {
            char c = _getch();
            if(note[now_note].GetState() >= 3)
                continue;
            if(setting.checkKey(c) == note[now_note].line)
            {
                switch(note[now_note].GetState())
                {
                    case 1: perfect_tot++; break;
                    case 2: great_tot++; break;
                }
                now_note++;
                continue;
            }
        }
    }
    
}

char output[3][105];
void PrintScreen()
{
    while(now_note <= note_cnt)
    {
        ClearScreen();
        cout << "Perfect\t\tGood\t\tMiss" << endl;
        cout << (int)perfect_tot << "\t\t" << (int)great_tot << "\t\t" << (int)miss_tot << endl;
        cout << "-----------------------------------------------" << endl;
        memset(output, 0, sizeof(output));
        for(int i=now_note;i<=can_seen;i++)
        {
            int pos = Speed * (note[i].time - NowTime()) + 2;
            if(pos <= 47) output[note[i].line][pos] = '<';
        }
        for(int i = 1; i <= 47; i++)
        {
            if(!output[1][i]) output[1][i] = ' ';
            if(!output[2][i]) output[2][i] = ' ';
        }
        output[1][48] = output[2][48] = '\n';
        output[1][1] = output[2][1] = '(';
        output[1][2] = output[2][2] = ')';
        strcat(output[1]+1, output[2]+1);
        cout << output[1]+1;
        cout << "===============================================" << endl;
        
        this_thread::sleep_for(chrono::milliseconds(20));
    }
    ClearScreen();
    cout << "Perfect\t\tGood\t\tMiss" << endl;
    cout << (int)perfect_tot << "\t\t" << (int)great_tot << "\t\t" << (int)miss_tot << endl;
}

// 0: Miss, 1: Perfect, 2: Great, 3: Far, 4: Cannot See
int note::GetState()
{
    int now_time = NowTime();
    if(now_time-time > Great) return 0;
    else if(abs(now_time-time) <= Prefect) return 1;
    else if(abs(now_time-time) <= Great) return 2;
    else if(time-now_time <= FallTime) return 3;
    else return 4;
}

bool LoadSpectrum()
{
    cout << "Loading spectrum..." << endl;
    Sleep(OneSecond);
    FILE* fr = fopen("data/music/spectrum.rbq","r");
    if(fr == nullptr)
    {
        cout << "No spectrum found" << endl;
        return false;
    }
    
    fscanf(fr, "%d", &note_cnt);
    for(int i = 1; i <= note_cnt; i++)
        fscanf(fr, "%d %d", &note[i].time, &note[i].line);
    
    fclose(fr);
    cout << "Spectrum loaded!" << endl;
    Sleep(OneSecond);
    ClearScreen();
    return true;
}

int NowTime()
{
    return clock()-start_time;
}


/*
Perfect         Bad             Miss
7777            7777            7777
-----------------------------------------------
()    < 
() <     <
===============================================
*/
