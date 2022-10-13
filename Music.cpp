#ifndef _MDLP_MUSIC
#define _MDLP_MUSIC

#include "header.cpp"

class Music{

    private:
        vector<pair<int,string> > list;
    public:
        void MusicMain();
        void Load();
        void PrintList();
        FILE* ChooseMusic();

}Music;

void Music::MusicMain(){

    PrintList();

    cout << "Press 'q' to quit.";
    while(1)
    {
        if(_kbhit())
        {
            char c=_getch();
            if(c=='q' or c=='Q') break;
        }
    }

}

void Music::Load(){

    ClearScreen();
    Print("Loading Music...\n", 20);

    string path=".\\data\\music\\";
    long long hFile = 0;
    struct _finddata_t fileinfo;
    string p;
    int cnt=0;
    if((hFile = _findfirst(p.assign(path).append("\\*.rbq").c_str(),&fileinfo)) !=  -1)
    {
        do
        {
            if(!(fileinfo.attrib &  _A_SUBDIR))
            {
                cnt++;
                list.emplace_back(cnt,fileinfo.name);
                cout<<"Loading "<<fileinfo.name<<endl;
            }
        }while(_findnext(hFile, &fileinfo)  == 0);
        _findclose(hFile);
    }
    
    Sleep(OneSecond/5);
    cout << "Loaded!" ;
    Sleep(OneSecond/5);

}

void Music::PrintList(){

    ClearScreen();
    printf("=============== Music List(total %d) ===============\n",list.size());

    for(int i=0;i<list.size();i++)
        cout<<list[i].first<<" "<<list[i].second<<endl;

    printf("===================================================\n");

}

FILE* Music::ChooseMusic(){

    begin:
    ClearScreen();
    PrintList();

    puts("Choose a music:");

    int id;
    cin>>id;

    if(id>list.size())
    {
        puts("Invalid ID!");
        Sleep(OneSecond);
        goto begin;
    }

    string path=".\\data\\music\\";
    path+=list[id-1].second;

    return fopen(path.data(),"r");

}

#endif // _MDLP_MUSIC
