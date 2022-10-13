/*
__    __     _____     __         ______  
/\ "-./  \   /\  __-.  /\ \       /\  == \ 
\ \ \-./\ \  \ \ \/\ \ \ \ \____  \ \  _-/ 
 \ \_\ \ \_\  \ \____-  \ \_____\  \ \_\   
  \/_/  \/_/   \/____/   \/_____/   \/_/   
version: v0.03
*/

#include "header.cpp"
#include "player.cpp"
#include "settings.cpp"
#include "recorder.cpp"
using namespace std;

int main()
{   
    Setting.CheckFiles();
    Setting.Load();
    while(true)
    {
        ClearScreen();
        cout << "===============================MDLP Main Menu=============================" << endl;
        cout << "1. Play(p)" << endl;
        cout << "2. Record(r)" << endl;
        cout << "3. Settings(s)" << endl;
        cout << "4. Quit(q)" << endl;
        cout << "==========================================================================" << endl;
        
        char c = WaitForInput();
        switch(c)
        {
            case 'p': case 'P': PlayerMain(); break;
            case 'r': case 'R': RecordMain(); break;
            case 's': case 'S': Setting.SettingsMain(); break;
            case 'q': case 'Q': goto end;
        }
    }
    
    end:
    
    ClearScreen();
    Print("Thanks for playing MDLP  >w<", 20);
}
