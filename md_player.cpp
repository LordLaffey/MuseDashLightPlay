/**
 * @details The player of MDLP
 * @author Ptilosis_w, LordLaffey
 * @version v1.05
 * @date 2022-10-17
*/

#include <vector>
#include <thread>
#include "include/console.h"
#include "include/header.h"
#include "include/song.h"
#include "music.cpp"
#include "settings.cpp"

using namespace std;

char WaitForInput();
void MDPrintScreen();
void MDPlayerMain();
void MDCheckKeys();
void MDChangeStatus(int);

HHOOK md_keyboardHook = 0;     // 钩子句柄
atomic<int> md_combo;
atomic<int> md_status,md_status_start;
atomic<bool> md_quit_flag;

void MDPlayerMain()
{
    ClearScreen();
    
    FILE* file = Music.ChooseMusic(1);
    if(!song.LoadSpectrum(file))
    {
        puts("No such file!");
        return void();
    }

    md_quit_flag = false;
    MDChangeStatus(-1);
    md_combo = 0;
    
    cout << "Press any key to start" << endl;
    WaitForInput();
    Print("Ready...\n", 9);
    Print("GO!!!\n", 6);
    
    con.open();
    start_time = clock();
    thread print(MDPrintScreen);
    thread check(MDCheckKeys);
    print.join();
    check.join();

    con.close();
    
    ClearScreen();
    
    if(song.miss_tot==0) Print("Full",5),Print(" Combo!",15);
    
    Sleep(OneSecond);
    ClearScreen();
    cout << "-----------------------------------------------" << endl;
    cout << "Perfect\t\tGreat\t\tMiss" << endl;
    cout << (int)song.perfect_tot << "\t\t" << (int)song.good_tot << "\t\t" << (int)song.miss_tot << endl;
    cout << "-----------------------------------------------" << endl;
    cout << "Ended" << endl;
    cout << "Press 'q' to return to the main menu" << endl;
    while(1)
    {
        if(!_kbhit()) continue;
        char c = _getch();
        if(c == 'q') break;
    }
    
    ClearScreen();
}

/**
 * @brief 监听按键回调函数
 * @param nCode 规定钩子如何处理消息，小于 0 则直接 CallNextHookEx
 * @param wParam 消息类型
 * @param lParam 指向某个结构体的指针，这里是 KBDLLHOOKSTRUCT（低级键盘输入事件）
*/
LRESULT CALLBACK MD_LowLevelKeyboardProc(_In_ int nCode, _In_ WPARAM wParam, _In_ LPARAM lParam)
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

    if(ks->vkCode == 27) md_quit_flag=true;
    int now = setting[0].checkKey(ks->vkCode);

    if(now!=-1)
    {
        switch(song.getStatus(now, (ks->flags == 0)))
        {
            case 1: song.perfect_tot++; MDChangeStatus(1); md_combo++; break;
            case 2: song.good_tot++; MDChangeStatus(2); md_combo++; break;
            case 3: song.miss_tot++; MDChangeStatus(0); md_combo = 0; break;
        }
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

void MDCheckKeys()
{
    // 安装钩子
    md_keyboardHook = SetWindowsHookEx(
        WH_KEYBOARD_LL,			// 钩子类型，WH_KEYBOARD_LL 为键盘钩子
        MD_LowLevelKeyboardProc,	// 指向钩子函数的指针
        GetModuleHandleA(NULL),	// Dll 句柄
        (DWORD)NULL
    );

    if (md_keyboardHook == 0) // 挂钩失败
    {
        md_quit_flag = true;// make md_printscreen quit
        puts("fuck hook");
        return ;
    }

    MSG msg;
    while(!song.isEnd())
    {
        bool tmp = song.run();
        if(tmp) MDChangeStatus(0),md_combo = 0;
        if(md_quit_flag) break;
        
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
    UnhookWindowsHookEx(md_keyboardHook);      // 删除钩子
}

void MDPrintScreen()
{
    static char output[3][105];
    static vector<Note> note;
    while(!song.isEnd())
    {
        if(md_quit_flag) return ;

        con << "Perfect\t\tGreat\t\tMiss" << endl;
        con << (int)song.perfect_tot << "\t\t" << (int)song.good_tot << "\t\t" << (int)song.miss_tot << endl;
        con << "-----------------------------------------------" << endl;
        
        memset(output, 0, sizeof(output));
        for(int i = 1; i <= 2; i++)
        {
            note = song.getNotes(i);
            for(auto v:note)
            {
                if(v.type == 1)
                {
                    int pos = MDSpeed * (v.start-NowTime()) + 2;
                    if(pos > 0 and pos <= 47) output[i][pos] = '<';
                }
                else
                {
                    int spos = MDSpeed * (v.start-NowTime()) + 2;
                    int epos = MDSpeed * (v.end-NowTime()) + 2;
                    if(spos > 0 and spos <= 47)
                        output[i][spos]='O';
                    else spos=0;
                    if(epos > 0 and epos <= 47)
                        output[i][epos]='O';
                    else if(epos>0) epos=48;
                    for(int j = spos+1; j < epos; j++)
                        output[i][j]='=';
                }
            }
        }

        // 拼接
        for(int i = 1; i <= 47; i++)
        {
            if(!output[1][i]) output[1][i] = ' ';
            if(!output[2][i]) output[2][i] = ' ';
        }
        output[1][48] = output[2][48] = '\n';
        output[1][1] = output[2][1] = '(';
        output[1][2] = output[2][2] = ')';
        strcat(output[1]+1, output[2]+1);
        con << output[1]+1;
        con << "===============================================" << endl;
        
        // 输出状态栏
        if(md_status != -1 && NowTime() - md_status_start <= Status_Time)
        {
            if(md_status==0) con << "Miss   ";
            else if(md_status==1) con << "Perfect";
            else if(md_status==2) con << "Great  ";
        }
        else con << "       ";
        if(md_combo>=5)
            con<<setw(45)<<"Combo: "<<md_combo<<endl;

        con.update();
        this_thread::sleep_for(chrono::milliseconds(20));
    }
    Sleep(OneSecond);
}

void MDChangeStatus(int status){

    md_status=status;
    md_status_start=NowTime();
    
}

/*
Perfect         Bad             Miss
7777            7777            7777
-----------------------------------------------
()    <      O===========O
() <     <
===============================================
*/
