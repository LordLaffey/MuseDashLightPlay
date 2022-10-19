#ifndef _MDLP_MUSIC
#define _MDLP_MUSIC

#include "include/header.h"
using namespace std;

class Music
{
    private:
        vector<pair<int, string>> list[2];
    
    public:
        void MusicPrework();
        void MusicMain();
        void Load(int type);
        void PrintList(int type);
        FILE *ChooseMusic(int type);
} Music;

void Music::MusicPrework()
{
    Load(1), Load(2);
}

void Music::MusicMain()
{
    ClearScreen();
    cout << "=========================MDLP Music Menu=======================" << endl;
    cout << "1. MuseDash Mode" << endl;
    cout << "2. 4K Mode" << endl;
    cout << "3. Back" << endl;
    cout << "===============================================================" << endl;
    
    int type = 1;
    while(true)
    {
        if(!_kbhit()) continue;
        char c = _getch();
        if(c == '1') { type = 1; break; }
        else if(c == '2') { type = 2; break; }
        else if(c == '3') return void();
    }
    
    PrintList(type);
    
    cout << "Press 'q' to quit.";
    while(true)
    {
        if(!_kbhit()) continue;
        char c = _getch();
        if(c == 'q' or c == 'Q') break;
    }
}

void Music::Load(int type)
{
    ClearScreen();
    string type_name = type == 1 ? "MuseDashMode" : "4KMode";
    Print("Loading ", 20);
    Print(type_name, 20);
    Print(" Music...\n", 20);
    type--;
    list[type].clear();
    
    string path = ".\\data\\music\\" + type_name + "\\";
    long long hFile = 0;
    struct _finddata_t fileinfo;
    string p;
    int cnt = 0;
    if((hFile = _findfirst(p.assign(path).append("\\*.rbq").c_str(), &fileinfo)) != -1)
    {
        do
        {
            if(!(fileinfo.attrib & _A_SUBDIR))
            {
                cnt++;
                list[type].emplace_back(cnt, fileinfo.name);
                cout << "Loading " << fileinfo.name << endl;
            }
        } while(_findnext(hFile, &fileinfo) == 0);
        _findclose(hFile);
    }
    
    Sleep(OneSecond / 5);
    cout << "Loaded!";
    Sleep(OneSecond / 5);
}

void Music::PrintList(int type)
{
    ClearScreen();
    type--;
    printf("=============== Music List(total %d) ===============\n", list[type].size());
    
    for(int i = 0; i < list[type].size(); i++)
        cout << list[type][i].first << " " << list[type][i].second << endl;
    
    printf("===================================================\n");
}

/**
* @param type 1: MuseDash Mode, 2: 4K Mode
*/
FILE *Music::ChooseMusic(int type)
{
    begin:
    ClearScreen();
    PrintList(type);
    
    type--;
    puts("Choose a music, input 114514 to quit:");
    
    int id; cin >> id;
    
    if(id == 114514) return NULL;
    
    if(id > list[type].size())
    {
        puts("Invalid ID!");
        Sleep(OneSecond);
        goto begin;
    }
    
    string type_name = type == 0 ? "MuseDashMode" : "4KMode";
    string path = ".\\data\\music\\";
    path += type_name + "\\" + list[type][id - 1].second;
    
    return fopen(path.data(), "r");
}

#endif // _MDLP_MUSIC
