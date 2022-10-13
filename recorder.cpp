/**
 * @details A recorder to record the Spectrum of MuseDashLightPlay.
 * @author LordLaffey
 * @version v1.11
*/

#include "header.cpp"
#include "settings.cpp"
using namespace std;

void About();
void Help();
bool PreExit();
void Record();
void RecordMain();
void Save(string name,int cnt);

void RecordMain()
{
    
    ClearScreen();
    Sleep(OneSecond/5);
    cout << "\nPress any key to continue..." << endl;
    WaitForInput();
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
        switch(c)
        {
            case 's':case 'S': Record(); break;
            case 'a':case 'A': About(); break;
            case 'q':case 'Q': return void(); break;
            default: break;
        }
    }
    
}


void About()
{
    begin:
    ClearScreen();
    Print("This is a project to record the Spectrum of MuseDashLightPlay.\n",20);
    Print("This program is written by C++.\n",20);
    Print("And it is coded by LingChen, LordLaffey and Ptilopsis_w.\n",20);
    Print("You can contact us by github:@LordLaffey, @qingchenling, @Ptilopsis_w.\n",20);
    
    if(PreExit()) return;
    else goto begin;
}

void Help()
{
    begin:
    ClearScreen();
    setting.printKey();
    Sleep(OneSecond/10);
    if(PreExit()) return ;
    else goto begin;
}

bool PreExit()
{
    puts("Press 'q' to quit...");   
    
    char c = WaitForInput();
    if(c == 'q' or c == 'Q')
        return true;
    else
        return false;
}

void Save(string name,int cnt)
{
    Print("Saveing...\n", 20);
    Sleep(OneSecond/2);
    FILE* fr = fopen(name.data(), "r");
    FILE* tmpw = fopen("data/music/tmp.rubbish", "w");
    
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
    
    FILE* fw = fopen(name.data(),"w");
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

void Record()
{
    ClearScreen();
    puts("Please input the name of the music:");
    string name;
    cin >> name;
    ClearScreen();
    name="data/music/"+name+".rbq";
    FILE* fw = fopen(name.data(),"w");
    cout << "Press any key to start recording..." << endl;
    
    WaitForInput();
    
    Print("3...\n", 5);
    Print("2...\n", 5);
    Print("1...\n", 5);
    cout << "Start!" << endl;
    
    ClearScreen();
    
    int start_time = clock();
    int cnt = 0;
    while(1)
    {
        if(_kbhit())
        {
            char c = _getch();
            if(c == 'q' or c == 'Q') break;
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
    }
    
    fclose(fw);
    Save(name,cnt);
}
