#ifndef _MDLP_SETTING
#define _MDLP_SETTING

#include "header.cpp"
#include <winspool.h>
using namespace std;


class SETTING{

    private:
        char key[4]={'d','f','j','k'};
    public:
        void CheckFiles();
        void Load();
        void SettingsMain();
        void ResetKey();
        void PrintKey();
        void SaveSettings();
        int CheckKey(char c);

}Setting;

int SETTING::CheckKey(char c){

    if(c==Setting.key[0]||c==Setting.key[0]-32)
        return 1;
    else if(c==Setting.key[1]||c==Setting.key[1]-32)
        return 1;
    else if(c==Setting.key[2]||c==Setting.key[2]-32)
        return 2;
    else if(c==Setting.key[3]||c==Setting.key[3]-32)
        return 2;
    else return -1;

}

void SETTING::CheckFiles(){

    Print("Checking the files...\n",20);
    Sleep(OneSecond/20);

    if(access("data",0)==-1)
        mkdir("data");
    if(access("data/music",0)==-1)
        mkdir("data/music");
    if(access("data/settings.laf",0)==-1)
    {
        FILE* fw=fopen("data/settings.laf","w");
        fprintf(fw,"d f j k");
        fclose(fw);
    }

    puts("Completed!");
    Sleep(OneSecond/20);

}

void SETTING::SettingsMain(){
    
    system("cls");
    Print("Settings:\n",20);
    cout << "1. Reset the keys" << endl;
    cout << "2. Print the keys" << endl;
    cout << "3. Back\n" << endl;
    while(true)
    {
        char c = WaitForInput();
        switch(c){
            case '1': Setting.ResetKey(); goto end;
            case '2': Setting.PrintKey(); goto end;
            case '3': goto end;
            default: break;
        }
    }
    end:;

}

void SETTING::Load(){
    
    ClearScreen();
    Print("Loading...\n",20);
    FILE* fr = fopen("data/settings.laf","r");
    fscanf(fr,"%c %c %c %c", &key[0], &key[1], &key[2], &key[3]);
    fclose(fr);
    cout << "Completed!" << endl;
    Sleep(OneSecond/2);
    ClearScreen();
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
    WaitForInput();
    
}

void SETTING::SaveSettings(){

    system("cls");
    Print("Saving...",20);
    FILE* fw=fopen("data/settings.laf","w");
    fprintf(fw,"%c %c %c %c",key[0],key[1],key[2],key[3]);
    fclose(fw);
    puts("The settings has saved!");

}




#endif // _MDLP_SETTING
