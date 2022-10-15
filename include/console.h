#ifndef _CONSOLE
#define _CONSOLE

#include <iostream>
#include <windows.h>
using namespace std;

class outbuf : public streambuf
{
    HANDLE h;
    char bytes[1024]="error: can't cin";
public:
    outbuf() {}
    outbuf(HANDLE h) : h(h) {}
protected:
    virtual int_type underflow() override
	{
        // DWORD readen;
        // ReadConsole(h, bytes, 1024, &readen, NULL);
        setg(bytes, bytes, bytes+1024);
		return *gptr();
	}
    virtual int_type overflow(int_type c) override
    {
        if (c != EOF)
        {
            DWORD written;
            WriteConsole(h, &c, 1, &written, NULL);
        }
        return c;
    }
    virtual streamsize xsputn(char_type const *s, streamsize count) override
    {
        DWORD written;
        WriteConsole(h, s, count, &written, NULL);
        return written;
    }
};

class Console
{
private:
    HANDLE out1,out2;
    outbuf ob1,ob2;
    void hideCur(HANDLE handle)
    {
        CONSOLE_CURSOR_INFO CursorInfo;
        GetConsoleCursorInfo(handle, &CursorInfo);
        CursorInfo.bVisible = false;
        SetConsoleCursorInfo(handle, &CursorInfo);
    }
    void cls(HANDLE handle)
    {
        COORD coordScreen = {0, 0};
        DWORD cCharsWritten;
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        DWORD dwConSize;
        
        GetConsoleScreenBufferInfo(handle, &csbi);
        dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
        FillConsoleOutputCharacter(handle, TEXT(' '), dwConSize, coordScreen, &cCharsWritten);
        GetConsoleScreenBufferInfo(handle, &csbi);
        FillConsoleOutputAttribute(handle, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
        SetConsoleCursorPosition(handle, coordScreen);
    }
public:
    Console()
    {
        out1=GetStdHandle(STD_OUTPUT_HANDLE);
        out2=CreateConsoleScreenBuffer(GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
        ob1=outbuf(out1); ob2=outbuf(out2);
        update();
        hideCur(out1); hideCur(out2);
    }
    void update()
    {
        swap(out1, out2); swap(ob1, ob2);
        SetConsoleActiveScreenBuffer(out1);
        cout.rdbuf(&ob2); cin.rdbuf(&ob1);
        cls(out2);
    }
    ~Console() { CloseHandle(out2); }
};

static Console console;

#endif // _CONSOLE
