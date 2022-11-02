/**
 * @file foutkey_player.cpp
 * @details The player of FourKey Mode
 * @authors LordLaffey, Ptilopsis_w, qingchenling
 * @version v1.06
 * @date 2022-11-2
 */

#include "include/header.h"
#include "include/console.h"
#include "include/song.h"
#include "settings.cpp"
#include "music.cpp"
using namespace std;

void FourKeyPlayerMain();
void FourkeyPrintScreen();
void FourKeyCheckKeys();
void FourKeyChangeStatus(int);
bool FourKeyPrework();
void FourKeyEndwork();

HHOOK fourkey_keyboardHook = 0;     // 钩子句柄
atomic<int> fourkey_combo;
atomic<int> fourkey_status,fourkey_status_start;
atomic<bool> fourkey_quit_flag;
void FourKeyPlayerMain()
{
    ClearScreen();
    if(!FourKeyPrework()) return ;
    
    cout << "Press any key to start" << endl;
    WaitForInput();
    Print("Ready...\n", 9);
    Print("GO!!!\n", 6);
    ClearScreen();

    con.open();
    start_time = clock();
    thread print(FourkeyPrintScreen);
    thread check(FourKeyCheckKeys);
    print.join();
    check.join();
    con.close();

    Sleep(OneSecond);
    FourKeyEndwork();

}

/**
 * @brief 监听按键回调函数
 * @param nCode 规定钩子如何处理消息，小于 0 则直接 CallNextHookEx
 * @param wParam 消息类型
 * @param lParam 指向某个结构体的指针，这里是 KBDLLHOOKSTRUCT（低级键盘输入事件）
*/
LRESULT CALLBACK FourKey_LowLevelKeyboardProc(_In_ int nCode, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
    KBDLLHOOKSTRUCT *ks = (KBDLLHOOKSTRUCT*)lParam;		// 包含低级键盘输入事件信息
    /*
    typedef struct tagKBDLLHOOKSTRUCT {
        DWORD     vkCode;		// 按键代号
        DWORD     scanCode;		// 硬件扫描代号，同 vkCode 也可以作为按键的代号。
        DWORD     flags;		// 事件类型，一般按键按下为 0 抬起为 128。
        DWORD     time;			// 消息时间戳
        ULONG_PTR dwExtraInfo;	// 消息附加信息，一般为 0。
    }KBDLLHOOKSTRUCT,*LPKBDLLHOOKSTRUCT,*PKBDLLHOOKSTRUCT;
    */

    if(ks->vkCode == 27) fourkey_quit_flag=true;
    int now = setting[1].checkKey(ks->vkCode);

    if(now!=-1)
    {
        switch(song.getStatus(now, (ks->flags == 0)))
        {
            case 1: song.perfect_tot++; FourKeyChangeStatus(1); fourkey_combo++; break;
            case 2: song.good_tot++; FourKeyChangeStatus(2); fourkey_combo++; break;
            case 3: song.bad_tot++; FourKeyChangeStatus(3); fourkey_combo = 0; break;
        }
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

void FourKeyCheckKeys()
{
    // 安装钩子
    fourkey_keyboardHook = SetWindowsHookEx(
        WH_KEYBOARD_LL,			// 钩子类型，WH_KEYBOARD_LL 为键盘钩子
        FourKey_LowLevelKeyboardProc,	// 指向钩子函数的指针
        GetModuleHandleA(NULL),	// Dll 句柄
        (DWORD)NULL
    );

    if (fourkey_keyboardHook == 0) // 挂钩失败
    {
        fourkey_quit_flag = true;// make md_printscreen quit
        puts("fuck hook");
        return ;
    }

    MSG msg;
    while(!song.isEnd())
    {
        bool tmp = song.run();
        if(tmp) FourKeyChangeStatus(0),fourkey_combo = 0;
        if(fourkey_quit_flag) break;
        
        // 如果消息队列中有消息
        if (PeekMessageA(
            &msg,		// MSG 接收这个消息
            NULL,		// 检测消息的窗口句柄，NULL：检索当前线程所有窗口消息
            (UINT)NULL,		// 检查消息范围中第一个消息的值，NULL：检查所有消息（必须和下面的同时为NULL）
            (UINT)NULL,		// 检查消息范围中最后一个消息的值，NULL：检查所有消息（必须和上面的同时为NULL）
            PM_REMOVE	// 处理消息的方式，PM_REMOVE：处理后将消息从队列中删除
            ))
        {
            TranslateMessage(&msg);     // 把按键消息传递给字符消息
            DispatchMessageW(&msg);     // 将消息分派给窗口程序
        }
        else
            Sleep(0);    //避免CPU全负载运行
    }
    UnhookWindowsHookEx(fourkey_keyboardHook);      // 删除钩子
}

/**
 * @brief 屏幕打印线程
*/
void FourkeyPrintScreen()
{
    static char output[20][40];
    static char buf[1000];
    static vector<Note> note;

    while(!song.isEnd())
    {
        if(fourkey_quit_flag) return ;
        con << "Perfect\tGood\tBad\tMiss" << endl;
        con << (int)song.perfect_tot << "\t" << (int)song.good_tot << "\t" 
            << (int)song.bad_tot << "\t" << (int)song.miss_tot << endl;
        con << "----------------------------" << endl;
        memset(output, 0, sizeof(output));
        for(int i = 1; i <= 4; i++)
        {
            note=song.getNotes(i);
            int st=(i-1)*8;
            for(auto v:note)
            {
                if(v.type==1)
                {
                    int pos = 15-FourKeySpeed * (v.start - NowTime())+2;
                    if(pos < 0 || pos > 15) continue;
                    for(int j = 1; j<=4; j++)
                        output[pos][st+j] = 'x';
                }
                else
                {
                    int spos = 15-FourKeySpeed * (v.start - NowTime())+2;
                    int epos = 15-FourKeySpeed * (v.end - NowTime())+2;
                    if(spos >= 0 and spos <= 15)
                    {
                        for(int j = 1; j<=4; j++)
                            output[spos][st+j] = '~';
                    }
                    else if(spos>15) spos=16;
                    if(epos >= 0 and epos <= 15)
                    {
                        for(int j = 1; j<=4; j++)
                            output[epos][st+j] = '~';
                    }
                    else if(epos<=15) epos=-1;
                    for(int j = epos+1; j < spos; j++)
                        output[j][st+1]='|',output[j][st+4]='|';
                }
            }
        }
        for(int i = 1; i <= 15; i++)
            for(int j = 1; j <= 30; j++)
                if(!output[i][j]) output[i][j] = ' ';
        
        memset(buf, 0, sizeof(buf));
        for(int i = 1; i <= 15; i++)
        {
            output[i][30] = '\n';
            strcat(buf+1, output[i]+1);
        }
        con << (buf+1);
        con << "====----====----====----====" << endl;
        if(fourkey_status != -1 && NowTime() - fourkey_status_start <= Status_Time)
        {
            if(fourkey_status == 0) con << "Miss   ";
            if(fourkey_status == 1) con << "Perfect";
            else if(fourkey_status == 2) con << "Good   ";
            else if(fourkey_status == 3) con << "Bad    ";
        }
        else con << "       ";
        pair<string,string> MS = GetMinuteSecond(song.GetAllTime()-NowTime());
        con << setw(6) << MS.first << ":" << MS.second;
        if(fourkey_combo >= 5) con << setw(11) << "Combo: " << fourkey_combo;
        con.update();
        this_thread::sleep_for(chrono::milliseconds(20));
    }
    
}

/**
 * @brief 更改实时响应状态
*/
void FourKeyChangeStatus(int status){
    
    fourkey_status = status;
    fourkey_status_start = NowTime();
    
}

/**
 * @brief To make the Main() function more clear.
 * @return 是否成功读取谱面
*/
bool FourKeyPrework(){
    
    FILE* fr = Music.ChooseMusic(2);
    if(!song.LoadSpectrum(fr))
    {
        Sleep(OneSecond);
        return false;
    }
    song.MakeHolds(1);
        
    fourkey_combo = 0;
    fourkey_quit_flag = false;
    FourKeyChangeStatus(-1);
    return true;
    
}

void FourKeyEndwork(){
    
    ClearScreen();
    cout << "Perfect\tGood\tBad\tMiss" << endl;
    cout << (int)song.perfect_tot << "\t" << (int)song.good_tot << "\t" 
        << (int)song.bad_tot << "\t" << (int)song.miss_tot << endl;
    cout << "----------------------------" << endl;
    cout << "Ended" <<endl;
    cout << "Press 'q' to return to the main menu" << endl;
    
    while(1)
    {
        if(!_kbhit()) continue;
        char c = _getch();
        if(c == 'q') break;
    }
    ClearScreen();
    
}

/*

Perfect Good    bad    miss
7777    7777    7777   7777
----------------------------
                ~~~~
                |  |
                |  |
                |  |
                |  |
                |  | 
                ~~~~
        xxxx
xxxx
        xxxx
                xxxx
                        xxxx
                xxxx
        xxxx
xxxx
====----====----====----====

*/