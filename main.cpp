/**
 *  __    __     _____     __         ______  
 * /\ "-./  \   /\  __-.  /\ \       /\  == \ 
 * \ \ \-./\ \  \ \ \/\ \ \ \ \____  \ \  _-/ 
 *  \ \_\ \ \_\  \ \____-  \ \_____\  \ \_\   
 *   \/_/  \/_/   \/____/   \/_____/   \/_/   
 * 
 * @version v0.05 
*/

#include "header.cpp"
#include "player.cpp"
#include "settings.cpp"
#include "recorder.cpp"
using namespace std;

int main()
{   
    ClearScreen();
    setting.load();
    Music.Load();
    while(true)
    {
        ClearScreen();
        cout << "===============================MDLP Main Menu=============================" << endl;
        cout << "1. Play(p)" << endl;
        cout << "2. Record(r)" << endl;
        cout << "3. Music(m)" <<endl;
        cout << "4. Settings(s)" << endl;
        cout << "5. Quit(q)" << endl;
        cout << "==========================================================================" << endl;
        
        char c = WaitForInput();
        switch(c)
        {
            case 'p': case 'P': PlayerMain(); break;
            case 'r': case 'R': RecordMain(); break;
            case 'm': case 'M': Music.MusicMain(); break;
            case 's': case 'S': SettingsMain(); break;
            case 'q': case 'Q': goto end;
        }
    }
    
    end:
    
    ClearScreen();
    Print("Thanks for playing MDLP  >w<", 20);
}
