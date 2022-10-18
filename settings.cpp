#ifndef _MDLP_SETTING
#define _MDLP_SETTING

#include <vector>
#include <set>
#include "include/header.h"

using namespace std;

/**
 * @details KeyChecker Class
*/
class Setting{
    
    private:
        vector<pair<char,int> > keys;
        string path;
        void write()
        {
            FILE* fw = fopen(path.data(),"w");
            for(auto key : keys)
                fprintf(fw, "%c %d\n", key.first, key.second);
            fclose(fw);
        }
        void read()
        {
            FILE* fr = fopen(path.data(),"r");
            keys.clear();
            char c; int x;
            while(fscanf(fr, "%c %d\n", &c, &x) != EOF)
                keys.emplace_back(c, x);
            fclose(fr);
        }
    public:
        void Prework(int typ){
            
            path=typ==0?"data/md_settings.laf":"data/fourkey_settings.laf";
            load();
            
        }
        void load()
        {
            ClearScreen();
            Print("Loading...\n", 20);
            
            if(access(path.data(),0) == -1)
            {
                if(path=="data/md_settings.laf")
                    keys = {{'d',1}, {'f',1}, {'j',2}, {'k',2}};
                else keys = {{'d',1}, {'f',2}, {'j',3}, {'k',4}};
                write();
            }
            
            read();
            puts("Completed!");
            Sleep(OneSecond/20);
        }
        int checkKey(char c)
        {
            for(auto key : keys)
                if(c == key.first or c == key.first-32)
                    return key.second;
            return -1;
        }
        void printKey()
        {
            ClearScreen();
            puts("Your key is:");
            for(auto key : keys)
                printf("%c %d\n", key.first, key.second);
            WaitForInput();
        }
        void set(vector<pair<char,int>> newkeys)
        {
            keys = newkeys;
            write();
        }
    
};

Setting setting[2];

void ResetKey(int);

void SettingsMain()
{
    begin:
    
    ClearScreen();
    
    puts("1. Muse Dash Setting.");
    puts("2. 4Key Setting.");
    puts("3. Back.");
    
    int type;
    while(1)
    {
        if(!_kbhit()) continue;
        char c = _getch();
        if(c=='1')
            {type = 1; break;}
        else if(c=='2')
            {type = 2; break;}
        else if(c=='3')
            {goto end; break;}
    }
    
    ClearScreen();
    Print(type==1?"Muse Dash ":"4Key ", 20);
    Print("Settings:\n",20);
    cout << "1. Reset the keys." << endl;
    cout << "2. Print the keys." << endl;
    cout << "3. Back.\n" << endl;
    
    type--;
    while(true)
    {
        if(!_kbhit()) continue;
        char c = _getch();
        switch(c)
        {
            case '1': ResetKey(type+1); goto end;
            case '2': setting[type].printKey(); goto end;
            case '3': goto begin; break;
            default: break;
        }
    }
    
    end:;
}

void ResetKey(int type)
{
    type--;
    vector<pair<char,int>> keys;
    set<char> st;
    
    begin:
    system("cls");
    puts(type?"4Key Settings:":"Muse Dash Settings:");
    puts("Input a char and a number on a line, you can input many until \"0\" \n");
    
    keys.clear();
    st.clear();
    
    while(true)
    {
        char c; int x;
        scanf("%c", &c);
        if(c == '0') break;
        scanf("%d\n", &x);
        
        c = lower(c);
        if(st.count(c))
        {
            puts("Error: The key cannot be the same!");
            continue;
        }
        keys.emplace_back(c, x);
    }
    
    puts("Sure to Save?(y/n)");
    while(true)
    {
        if(!_kbhit()) continue;
        char c = _getch();
        if(c == 'n' or c == 'N') goto begin;
        else if(c == 'y' or c == 'Y') break;
    }
    
    puts("Saving...");
    Sleep(OneSecond);
    setting[type].set(keys);
    puts("The settings has saved!");
    Sleep(OneSecond);
}

#endif // _MDLP_SETTING
