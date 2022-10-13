/*
__    __     _____     __         ______  
/\ "-./  \   /\  __-.  /\ \       /\  == \ 
\ \ \-./\ \  \ \ \/\ \ \ \ \____  \ \  _-/ 
 \ \_\ \ \_\  \ \____-  \ \_____\  \ \_\   
  \/_/  \/_/   \/____/   \/_____/   \/_/   
version: v0.01
*/

#include <iostream>
#include "header.cpp"
#include "player.cpp"
#include "recorder.cpp"
using namespace std;

int main()
{
    while(1)
    {
        ClearScreen();
        cout << "Main Menu:" << endl;
        cout << "----------------------------------" << endl;
        cout << "Record(r)" << endl;
        cout << "Play(p)" << endl;
        cout << "Quit(q)" << endl;
        char c = WaitForInput();
        switch(c)
        {
            case 'r': RecordMain(); break;
            case 'p': PlayerMain(); break;
            case 'q': goto end;
        }
    }
    
    end:;
}
