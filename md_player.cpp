/**
 * @details The player of MDLP
 * @author Ptilosis_w, LordLaffey
 * @version v0.05
*/

#include "header.cpp"
#include "settings.cpp"
#include "music.cpp"

using namespace std;
using namespace song;

char WaitForInput();
void MDPrintScreen();
void MDCheckKeys();
void MDPlayerMain();

void MDPlayerMain()
{
    ClearScreen();

    FILE* file = Music.ChooseMusic(1);
    if(!LoadSpectrum(file)) return void();
    
    cout << "Press any key to start" << endl;
    WaitForInput();
    Print("Ready...\n", 9);
    Print("GO!!!\n", 6);
    
    song::reset();
    
    start_time = clock();
    thread print(MDPrintScreen);
    thread check(MDCheckKeys);
    print.join(); check.join();
    
    cout << "-----------------------------------------------" << endl;
    cout << "Ended" << endl;
    cout << "Press any key return to the main menu" << endl;
    WaitForInput();
    ClearScreen();
}

void MDCheckKeys()
{
    now_note = 1;
    while(now_note <= note_cnt)
    {
        while(now_note <= note_cnt && !note[now_note].GetState())
            miss_tot++,now_note++;
        while(can_seen <= note_cnt && note[can_seen].GetState() != 5)
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
                    case 2: good_tot++; break;
                }
                now_note++;
                continue;
            }
        }
    }
    
}

char output[3][105];
void MDPrintScreen()
{
    while(now_note <= note_cnt)
    {
        ClearScreen();
        cout << "Perfect\t\tGreat\t\tMiss" << endl;
        cout << (int)perfect_tot << "\t\t" << (int)good_tot << "\t\t" << (int)miss_tot << endl;
        cout << "-----------------------------------------------" << endl;
        memset(output, 0, sizeof(output));
        for(int i=now_note;i<=can_seen;i++)
        {
            int pos = MD_Speed * (note[i].time-NowTime()) + 2;
            if(pos > 0 and pos <= 47) output[note[i].line][pos] = '<';
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
    Sleep(OneSecond);
    cout << "Perfect\t\tGreat\t\tMiss" << endl;
    cout << (int)perfect_tot << "\t\t" << (int)good_tot << "\t\t" << (int)miss_tot << endl;
}





/*
Perfect         Bad             Miss
7777            7777            7777
-----------------------------------------------
()    < 
() <     <
===============================================
*/
