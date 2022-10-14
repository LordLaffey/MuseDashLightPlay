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
void MDPlayerMain();
template<int id>
void MDCheckKeys();

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
    track[1].init(1);
    track[2].init(2);
    
    start_time = clock();
    thread print(MDPrintScreen);
    thread check1(MDCheckKeys<1>);
    thread check2(MDCheckKeys<2>);
    print.join();
    check1.join();
    check2.join();
    
    cout << "-----------------------------------------------" << endl;
    cout << "Ended" << endl;
    cout << "Press any key return to the main menu" << endl;
    WaitForInput();
    ClearScreen();
}

template<int id>
void MDCheckKeys()
{
    Track &t = track[id];
    while(t.now_note <= t.note_cnt)
    {
        while(t.now_note <= t.note_cnt and !GetNoteState(t.note[t.now_note]))
            miss_tot++, t.now_note++, song::now_note++;
        while(t.can_seen <= t.note_cnt and GetNoteState(t.note[t.can_seen]) != 5)
            t.can_seen++;
        if(!_kbhit()) continue;
        char c = _getch();
        if(GetNoteState(t.note[t.now_note]) >= 3) continue;
        if(setting.checkKey(c) == id)
        {
            switch(GetNoteState(t.note[t.now_note]))
            {
                case 1: perfect_tot++; break;
                case 2: good_tot++; break;
            }
            t.now_note++;
            song::now_note++;
        }
    }
}

char output[3][105];
void MDPrintScreen()
{
    while(song::now_note <= song::note_cnt)
    {
        ClearScreen();
        cout << "Perfect\t\tGreat\t\tMiss" << endl;
        cout << (int)perfect_tot << "\t\t" << (int)good_tot << "\t\t" << (int)miss_tot << endl;
        cout << "-----------------------------------------------" << endl;
        memset(output, 0, sizeof(output));
        for(int i = 1; i <= 2; i++)
        {
            const Track &t = track[i];
            for(int j = t.now_note; j < t.can_seen; j++)
            {
                int pos = MD_Speed * (t.note[j]-NowTime()) + 2;
                if(pos > 0 and pos <= 47) output[i][pos] = '<';
            }
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
