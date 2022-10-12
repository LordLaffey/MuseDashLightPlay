/*
A recorder to record the Spectrum of OSU!.
Author: LordLaffey
License: GNU General Public License
Source: http://www.github.com/LordLaffey/OBQ/
Language: cpp
LastEditor: 2022/10/12
version: v0.02
*/

#include<bits/stdc++.h>
#include<windows.h>
#include<conio.h>
#include<io.h>
using namespace std;
const int OneSecond=1000;

char lower(char c){return c>=65&&c<=90?c+32:c;}

void Print(string s,int times);
void Welcome();
bool Quit(bool confirm = false);
bool PreExit();
void StartScreen();
void Save(int cnt);
void Record();
void Thanks();
void Help();
void About();
int GetKeyToRecord(char c);

class SETTING{

    private:
        char key[4]={'d','f','j','k'};
    public:
        void Prework();
        void Load();
        void ResetKey();
        void PrintKey();
        void SaveSettings();
        int CheckKey(char c);

}Setting;

int main(){

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
                case 'q':case 'Q': if(Quit(true)) return Thanks(),0;break;
                case 'h':case 'H': Help(),StartScreen();break;
                case 'a':case 'A': About(),StartScreen();break;
                case 'r':case 'R': Setting.ResetKey(),StartScreen();break;
                default: break;
            }
        }
    }

    return 0;

}

void Print(string s,int times){

    
    for(auto v:s)
    {
        cout<<v;
        if(_kbhit())
        {
            char c=_getch();
            times = INT_MAX;
        }
        Sleep(OneSecond/times);
    }

}

void Welcome(){

    system("cls");
    string Welcome="Welcome to OSU!";

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
    Print("This is a project to record the Spectrum of OSU!\n",20);
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
    
    FILE* fr=fopen("data/spectrum.rbq","r");
    FILE* tmpw=fopen("data/tmp.rubbish","w");
    
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
    
    FILE* fw=fopen("data/spectrum.rbq","w");
    FILE* tmpr=fopen("data/tmp.rubbish","r");
    
    fprintf(fw,"%d\n",cnt);
    
    for(int i=1;i<=cnt;i++)
    {
        fscanf(tmpr,"%s ",s1);
        fscanf(tmpr,"%s\n",s2);
        fprintf(fw,"%s %s\n",s1,s2);
    }
    
    fclose(fw);
    fclose(tmpr);
    
    system("del data/tmp.rubbish");
    
}

void Thanks(){
    
    Print("Thanks for playing OSU!>w<", 20);
    
}

void Record(){

    system("cls");
    FILE* fw=fopen("data/spectrum.rbq","w");

    puts("Press any key to start recording...");

    while(1)
    {
        if(_kbhit())
        {
            char c=_getch();
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

    Print("Saveing...\n",20);
    Sleep(OneSecond/2);
    Save(cnt);
    puts("Saved!");
    Sleep(OneSecond/2);

}

int SETTING::CheckKey(char c){

    if(c==Setting.key[0]||c==Setting.key[0]-32)
        return 1;
    else if(c==Setting.key[1]||c==Setting.key[1]-32)
        return 2;
    else if(c==Setting.key[2]||c==Setting.key[2]-32)
        return 3;
    else if(c==Setting.key[3]||c==Setting.key[3]-32)
        return 4;
    else return -1;

}

void SETTING::Prework(){

    Print("Checking the setting...\n",20);
    Sleep(OneSecond/20);

    if(access("data",0)==-1)
        system("mkdir data");
    if(access("data/settings.laf",0)==-1)
    {
        FILE* fw=fopen("data/settings.laf","w");
        fprintf(fw,"d f j k");
        fclose(fw);
    }

    puts("Completed!");
    Sleep(OneSecond/20);

}

void SETTING::Load(){
    
    Print("Loading...",20);
    FILE* fr=fopen("data/settings.laf","r");
    fscanf(fr,"%c %c %c %c",key[0],key[1],key[2],key[3]);
    fclose(fr);
    puts("Completed!");
    
}

void SETTING::ResetKey(){

    begin:
    system("cls");
    puts("Input a string just like \"dfjk\" to reset the key.");

    string s;
    getline(cin, s);
    puts("Sure?(y/n)");
    while(1)
    {
        if(_kbhit())
        {
            char c=_getch();
            if(c=='n'||c=='N')
                goto begin;
            else if(c=='y'||c=='Y')
                break;
        }
    }
    if(s[0]==s[1]||s[0]==s[2]||s[0]==s[3]||s[1]==s[2]||s[1]==s[3]||s[2]==s[3])
    {
        puts("Error: The key cannot be the same!");
        Sleep(OneSecond);
        goto begin;
    }
    key[0]=lower(s[0]),key[1]=lower(s[1]),key[2]=lower(s[2]),key[3]=lower(s[3]);
    printf("Your key is '%c','%c','%c','%c' now.\n",key[0],key[1],key[2],key[3]);

    Sleep(OneSecond);
    SaveSettings();

}

void SETTING::PrintKey(){

    puts("Your key is:");
    printf("'%c','%c','%c','%c'\n",key[0],key[1],key[2],key[3]);

}

void SETTING::SaveSettings(){

    system("cls");
    Print("Saving...",20);
    FILE* fw=fopen("data/settings.laf","w");
    fprintf(fw,"%c %c %c %c",key[0],key[1],key[2],key[3]);
    fclose(fw);
    puts("The settings has saved!");

}

