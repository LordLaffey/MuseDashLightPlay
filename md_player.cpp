/**
 * @details The player of MDLP
 * @author Ptilosis_w, LordLaffey
 * @version v0.06
*/

#include "header.cpp"
#include "settings.cpp"
#include "music.cpp"

using namespace std;
using namespace song;

char WaitForInput();
void MDPrintScreen();
void MDPlayerMain();
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
    thread check(MDCheckKeys);
    print.join();
    check.join();

    ClearScreen();
}

atomic<bool> md_quit_flag;
void MDCheckKeys()
{
    while(song::now_note <= song::note_cnt)
    {
        for(int i=1;i<=2;i++)
        {
            Track &t = track[i];
            while(t.now_note <= t.note_cnt and !GetNoteState(t.note[t.now_note]))
                miss_tot++, t.now_note++, song::now_note++;
            while(t.can_seen <= t.note_cnt and GetNoteState(t.note[t.can_seen]) != 5)
                t.can_seen++;
        }
        if(!_kbhit()) continue;
        char c = _getch();
        if(c == 27) return md_quit_flag=true,void();
        int now=setting.checkKey(c);
        Track &t=track[now];
        if(GetNoteState(t.note[t.now_note]) >= 3) continue;
        switch(GetNoteState(t.note[t.now_note]))
        {
            case 1: perfect_tot++; break;
            case 2: good_tot++; break;
        }
        t.now_note++;
        song::now_note++;
    }
}

char output[3][105];
void MDPrintScreen()
{
    while(song::now_note <= song::note_cnt)
    {
        if(md_quit_flag) return ;
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
                int pos = MDSpeed * (t.note[j]-NowTime()) + 2;
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
    cout << "-----------------------------------------------" << endl;
    cout << "Ended" << endl;
    cout << "Press any key return to the main menu" << endl;
    WaitForInput();
}

/*
Perfect         Bad             Miss
7777            7777            7777
-----------------------------------------------
()    < 
() <     <
===============================================
*/
