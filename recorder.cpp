/**
 * @details A recorder to record the Spectrum of MuseDashLightPlay.
 * @author LordLaffey
 * @version v1.13
 */

#include "include/console.h"
#include "include/header.h"
#include "settings.cpp"
#include "music.cpp"

using namespace std;

void About();
void Help();
void Record();
void RecordMain();
void Save(string name, int cnt, int len, int type);

void RecordMain()
{

    ClearScreen();
    
    while(true)
    {
        ClearScreen();
        cout << "==========================MDLP Spectrum Recorder==========================" << endl;
        cout << "1. Start recording(s)" << endl;
        cout << "2. About(a)" << endl;
        cout << "3. Exit(q)" << endl;
        cout << "==========================================================================" << endl;
        
        char c = WaitForInput();
        switch(lower(c))
        {
            case 's': Record(); break;
            case 'a': About(); break;
            case 'q': case 27: return void(); break;
            default: break;
        }
    }
}

void About()
{
    ClearScreen();
    Print("This is a project to record the Spectrum of MuseDashLightPlay.\n", 20);
    Print("This program is written by C++.\n", 20);
    Print("And it is coded by LingChen, LordLaffey and Ptilopsis_w.\n", 20);
    Print("You can contact us by github:@LordLaffey, @qingchenling, @Ptilopsis_w.\n", 20);
    WaitForInput();
}

void Help()
{
    ClearScreen();
    setting.printKey();
    Sleep(OneSecond/10);
    WaitForInput();
}

void Save(string name, int cnt, int len, int type)
{
    ClearScreen();
    Print("Saveing...\n", 20);
    Sleep(OneSecond / 2);
    FILE *fr = fopen(name.data(), "r");
    FILE *tmpw = fopen("data/music/tmp.rubbish", "w");
    
    if(fr == nullptr)
    {
        cout << "Error: Cannot open file!" << endl;
        return void();
    }
    
    char s1[1000], s2[1000];
    for(int i = 1; i <= cnt; i++)
    {
        fscanf(fr, "%s ", s1);
        fscanf(fr, "%s\n", s2);
        fprintf(tmpw, "%s %s\n", s1, s2);
    }
    
    fclose(fr);
    fclose(tmpw);
    
    FILE *fw = fopen(name.data(), "w");
    FILE *tmpr = fopen("data/music/tmp.rubbish", "r");
    
    fprintf(fw, "%d %d %d\n", cnt, len, type);
    
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
    Sleep(OneSecond / 2);
    
    puts("Reloading music list...");
    Music.Load(type);
}

void Record()
{
    ClearScreen();
    cout << "Please input the name of the music:" << endl;
    string name;
    cin >> name;
    
    ChooseType:
    ClearScreen();
    
    cout << "Please input the type of the music:" << endl;
    cout << "1. MuseDash mode" << endl;
    cout << "2. 4K mode" << endl;
    
    int type;
    cin >> type;
    string type_name = type == 1 ? "MuseDashMode" : "4KMode";
    
    name = "data/music/" + type_name + "/" + name + ".rbq";
    FILE *fw = fopen(name.data(), "w");
    
    if(type != 1 && type != 2)
    {
        puts("No such type!");
        Sleep(OneSecond / 2);
        goto ChooseType;
    }
    
    ClearScreen();
    cout << "Press any key to start recording..." << endl;
    
    WaitForInput();
    
    Print("3...\n", 5);
    Print("2...\n", 5);
    int start_time = clock();
    Print("1...\n", 5);
    cout << "Start!" << endl;
    
    ClearScreen();
    
    int cnt = 0;
    while(true)
    {
        if(!_kbhit()) continue;
        char c = _getch();
        if(c == 'q' or c == 'Q')
            break;
        int key = setting.checkKey(c);
        if(key != -1)
        {
            fprintf(fw, "%d ", clock()-start_time);
            fprintf(fw, "%d\n", key);
            printf("%dms ", clock()-start_time);
            printf("%d\n", key);
            cnt++;
        }
    }
    
    fclose(fw);
    Save(name, cnt, clock()-start_time, type);
}
