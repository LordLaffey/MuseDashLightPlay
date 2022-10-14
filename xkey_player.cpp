/**
 * @file xkey_player.cpp
 * @authors LordLaffey, Ptilopsis_w, laplace-oo
 * @version v0.1
 * @date 2022-10-13
 */

#include "header.cpp"
#include "settings.cpp"
#include "music.cpp"
using namespace std;
using namespace song;


void Xkey_Main();
void XkeyPrintScreen();

template<int id>
void XkeyCheckKeys();

void Xkey_Main()
{
    ClearScreen();
    
    FILE* fr = Music.ChooseMusic(2);
    if(!LoadSpectrum(fr))
    {
        cout << "No such file!" << endl;
        Sleep(OneSecond);
        return void();
    }
    
    song::reset();
    
    start_time = 0;
    thread print(XkeyPrintScreen);
    thread check1(XkeyCheckKeys<1>);
    thread check2(XkeyCheckKeys<2>);
    thread check3(XkeyCheckKeys<3>);
    thread check4(XkeyCheckKeys<4>);
    print.join(); 
    check1.join(); 
    check2.join();
    check3.join();
    check4.join();
}

template<int id>
void XkeyCheckKeys()
{
    
}

void XkeyPrintScreen(){
    
    
    
}

/* UI

Perfect Good    bad    miss
7777    7777    7777   7777
----------------------------
30lines
                        xxxx
                xxxx
        xxxx
xxxx
====----====----====----====
key1    key2    key3    key4

*/
