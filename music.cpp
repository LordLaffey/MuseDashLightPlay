#ifndef _MDLP_MUSIC
#define _MDLP_MUSIC

#include "header.cpp"

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

// type = [1: MuseDash Mode | 2: 4K Mode]
FILE *Music::ChooseMusic(int type)
{
    begin:
    ClearScreen();
    PrintList(type);
    
    type--;
    puts("Choose a music:");
    
    int id; cin >> id;
    
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

namespace song {
    const int Prefect = 80;
    const int Great = 180;
    const int Bad = 250;
    
    int note_cnt;
    atomic<int> now_note;
    atomic<int> perfect_tot, good_tot, bad_tot, miss_tot;
    struct note {
        int time, line;
    } note[100000];
    void reset()
    {
        now_note = 1;
        perfect_tot = 0;
        good_tot = 0;
        miss_tot = 0;
    }
    bool LoadSpectrum(FILE *fr)
    {
        reset();
        cout << "Loading spectrum..." << endl;
        Sleep(OneSecond);
        if(fr == nullptr)
        {
            cout << "No spectrum found" << endl;
            Sleep(OneSecond);
            return false;
        }
        
        int tmp,type;
        fscanf(fr, "%d %d %d", &note_cnt, &tmp, &type);
        for(int i = 1; i <= note_cnt; i++)
            fscanf(fr, "%d %d", &note[i].time, &note[i].line);
        
        fclose(fr);
        cout << "Spectrum loaded!" << endl;
        Sleep(OneSecond);
        ClearScreen();
        return true;
    }
    
    // 0: Miss, 1: Perfect, 2: Great, 3: Bad, 4: Far, 5: Cannot See
    int GetNoteState(int x)
    {
        int now_time = NowTime();
        if(now_time-x > Bad) return 0;
        else if(abs(now_time-x) <= Prefect) return 1;
        else if(abs(now_time-x) <= Great) return 2;
        else if(abs(now_time-x) <= Bad) return 3;
        else if(x-now_time <= MDFallTime) return 4;
        else return 5;
    }
    
    struct Track
    {
        int note_cnt;
        atomic<int> now_note, can_seen;
        vector<int> note;
        void init(int id)
        {
            reset(); note.push_back(114514);
            for(int i = 1; i <= song::note_cnt; i++)
                if(song::note[i].line == id)
                    note.push_back(song::note[i].time);
            note_cnt = note.size()-1;
        }
        void reset()
        {
            now_note = 1;
            can_seen = 1;
            note.clear();
        }
    } track[5];
}

#endif // _MDLP_MUSIC
