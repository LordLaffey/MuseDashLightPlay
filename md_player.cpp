/**
 * @details The player of MDLP
 * @author Ptilosis_w, LordLaffey
 * @version v1.05
 * @date 2022-10-17
*/

#include <vector>
#include <thread>
#include "include/console.h"
#include "include/header.h"
#include "include/song.h"
#include "music.cpp"
#include "settings.cpp"

using namespace std;

char WaitForInput();
void MDPrintScreen();
void MDPlayerMain();
void MDCheckKeys();
void MDChangeStatus(int);

atomic<int> md_combo;
atomic<int> md_status,md_status_start;
atomic<bool> md_quit_flag;
void MDPlayerMain()
{
    ClearScreen();
    
    FILE* file = Music.ChooseMusic(1);
    if(!song.LoadSpectrum(file))
    {
        puts("No such file!");
        return void();
    }

    md_quit_flag = false;
    MDChangeStatus(-1);
    md_combo = 0;
    
    cout << "Press any key to start" << endl;
    WaitForInput();
    Print("Ready...\n", 9);
    Print("GO!!!\n", 6);
    
    con.open();
    start_time = clock();
    thread print(MDPrintScreen);
    thread check(MDCheckKeys);
    print.join();
    check.join();

    con.close();
    
    ClearScreen();
    
    if(song.miss_tot==0) Print("Full",5),Print(" Combo!",15);
    
    Sleep(OneSecond);
    ClearScreen();
    cout << "-----------------------------------------------" << endl;
    cout << "Perfect\t\tGreat\t\tMiss" << endl;
    cout << (int)song.perfect_tot << "\t\t" << (int)song.good_tot << "\t\t" << (int)song.miss_tot << endl;
    cout << "-----------------------------------------------" << endl;
    cout << "Ended" << endl;
    cout << "Press 'q' to return to the main menu" << endl;
    while(1)
    {
        if(!_kbhit()) continue;
        char c = _getch();
        if(c == 'q') break;
    }
    
    ClearScreen();
}

void MDCheckKeys()
{
    while(!song.isEnd())
    {
        bool tmp = song.run();
        if(tmp) MDChangeStatus(0),md_combo = 0;

        if(!_kbhit()) continue;
        char c = _getch();
        if(c == 27) return md_quit_flag=true,void();
        int now=setting.checkKey(c);
        if(now==-1) continue;
        
        switch(song.getStatus(now))
        {
            case 1: song.perfect_tot++; MDChangeStatus(1);md_combo++; break;
            case 2: song.good_tot++; MDChangeStatus(2);md_combo++; break;
        }
    }
}

void MDPrintScreen()
{
    static char output[3][105];
    static vector<int> note;
    while(!song.isEnd())
    {
        if(md_quit_flag) return ;
        con << "Perfect\t\tGreat\t\tMiss" << endl;
        con << (int)song.perfect_tot << "\t\t" << (int)song.good_tot << "\t\t" << (int)song.miss_tot << endl;
        con << "-----------------------------------------------" << endl;
        memset(output, 0, sizeof(output));
        for(int i = 1; i <= 2; i++)
        {
            note = song.getNotes(i);
            for(auto v:note)
            {
                int pos = MDSpeed * (v-NowTime()) + 2;
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
        con << output[1]+1;
        con << "===============================================" << endl;
        if(md_status != -1 && NowTime() - md_status_start <= Status_Time)
        {
            if(md_status==0) con << "Miss   ";
            else if(md_status==1) con << "Perfect";
            else if(md_status==2) con << "Great  ";
        }
        else con << "       ";
        if(md_combo>=5)
            con<<setw(45)<<"Combo: "<<md_combo<<endl;
        con.update();
        this_thread::sleep_for(chrono::milliseconds(20));
    }
    Sleep(OneSecond);
}

void MDChangeStatus(int status){

    md_status=status;
    md_status_start=NowTime();
    
}

/*
Perfect         Bad             Miss
7777            7777            7777
-----------------------------------------------
()    < 
() <     <
===============================================
*/
