/*
A recorder to record the Spectrum of MuseDashLightPlay.
Author: LordLaffey
License: GNU General Public License
Source: http://www.github.com/LordLaffey/MuseDashLightPlay/
Language: cpp
LastEditor: 2022/10/13
version: v1.04
*/

#include <bits/stdc++.h>
#include <windows.h>
#include <conio.h>
#include <io.h>
#include "header.cpp"
#include "settings.cpp"
using namespace std;

void About();
void Help();
bool PreExit();
void Print(string s,int times);
bool Quit(bool confirm = false);
void Record();
void RecordMain();
void Save(int cnt);
void StartScreen();
void Thanks();
void Welcome();

void RecordMain(){

    Setting.Prework();
    Welcome();
    StartScreen();

    while(1)
    {
        if(_kbhit())
        {
            char c;
            c=_getch();
            switch(c)
            {
                case 's':case 'S': Record(),StartScreen();break;
                case 'q':case 'Q': if(Quit(true)) return Thanks(); break;
                case 'h':case 'H': Help(),StartScreen();break;
                case 'a':case 'A': About(),StartScreen();break;
                case 'r':case 'R': Setting.ResetKey(),StartScreen();break;
                default: break;
            }
        }
    }

}


void Welcome(){

    system("cls");
    string Welcome="Welcome to MDLP!";

    Print(Welcome,20);
    Sleep(OneSecond/5);

    puts("\nPress any key to continue...");

    while(1)
    {
        if(_kbhit())
        {
            char c=_getch();
            system("cls");
            break;
        }
    }

}

void About(){

    begin:
    system("cls");
    string s;
    Print("This is a project to record the Spectrum of MuseDashLightPlay!\n",20);
    Print("This program is written by C++.\n",20);
    Print("And it is coded by LingChen, LordLaffey and Ptilopsis_w.\n",20);
    Print("You can contact us by github:@LordLaffey,@qingchenling,@Ptilopsis_w\n",20);
    
    if(PreExit()) return;
    else goto begin;

}

bool Quit(bool confirm){

    system("cls");
    if(!confirm) return true;
    puts("Do you want to quit? (Y/N)");
    while(1)
    {
        if(_kbhit())
        {
            char c=_getch();
            if(c=='Y'||c=='y')
                return true;
            else if(c=='N'||c=='n')
                return false;
        }
    }
    return false;

}

void Help(){

    begin:
    system("cls");
    Setting.PrintKey();
    Sleep(OneSecond/10);

    if(PreExit()) return ;
    else goto begin;

}

bool PreExit(){

    puts("Press 'q' to quit...");
    while(1)
    {
        if(_kbhit())
        {
            char c=_getch();
            if(c=='q'||c=='Q')
            {
                if(Quit())
                    return true;
                else return false;
            }
        }
    }
    return false;

}

void StartScreen(){

    system("cls");
    puts("========================The OSU Spectrum Recorder========================");
    puts("1. Start recording(s)");
    puts("2. Exit(q)");
    puts("3. Help(h)");
    puts("4. About(a)");
    puts("5. Reset the key(r)");
    puts("=========================================================================");

}

void Save(int cnt){
    
    Print("Saveing...\n",20);
    Sleep(OneSecond/2);
    FILE* fr=fopen("data/music/spectrum.rbq","r");
    FILE* tmpw=fopen("data/music/tmp.rubbish","w");

    if(fr == nullptr)
    {
        puts("Error: Cannot open file!");
        return;
    }
    
    char s1[1000],s2[1000];
    for(int i=1;i<=cnt;i++)
    {
        fscanf(fr,"%s ",s1);
        fscanf(fr,"%s\n",s2);
        fprintf(tmpw,"%s %s\n",s1,s2);
    }
    
    fclose(fr);
    fclose(tmpw);
    
    FILE* fw = fopen("data/music/spectrum.rbq","w");
    FILE* tmpr = fopen("data/music/tmp.rubbish","r");
    
    fprintf(fw, "%d\n", cnt);

    for(int i = 1; i <= cnt; i++)
    {
        fscanf(tmpr, "%s ", s1);
        fscanf(tmpr, "%s\n", s2);
        fprintf(fw, "%s %s\n", s1, s2);
    }
    
    fclose(fw);
    fclose(tmpr);
    
    system("del data\\music\\tmp.rubbish");
    
    puts("Saved!");
    Sleep(OneSecond/2);
    
}

void Thanks(){
    
    Print("Thanks for playing MDLP!>w<", 20);
    
}

void Record(){

    system("cls");
    FILE* fw = fopen("data/music/spectrum.rbq","w");

    puts("Press any key to start recording...");

    while(1)
    {
        if(_kbhit())
        {
            char c = _getch();
            break;
        }
    }

    Print("3...\n",5);
    Print("2...\n",5);
    Print("1...\n",5);
    puts("Start!");

    system("cls");

    int start_time=clock();
    int cnt=0;
    while(1)
    {
        if(_kbhit())
        {
            char c=_getch();
            if(c=='q'||c=='Q')
            {
                if(Quit())
                    break;
            }
            int key=Setting.CheckKey(c);
            if(key!=-1)
            {
                fprintf(fw,"%d ",clock()-start_time);
                fprintf(fw,"%d\n",key);
                printf("%dms ",clock()-start_time);
                printf("%d\n",key);
                cnt++;
            }
        }
    }

    fclose(fw);

    Save(cnt);

}
