#ifndef _CONSOLE
#define _CONSOLE

#include <iostream>
#include <windows.h>

class outbuf : public std::streambuf
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
    virtual std::streamsize xsputn(char_type const *s, std::streamsize count) override
    {
        DWORD written;
        WriteConsole(h, s, count, &written, NULL);
        return written;
    }
};

class Console : public std::ostream
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
        SetConsoleCursorPosition(handle, coordScreen);
    }
public:
    Console()
    {
        out1=CreateConsoleScreenBuffer(GENERIC_READ|GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
        out2=CreateConsoleScreenBuffer(GENERIC_READ|GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
        ob1=outbuf(out1); ob2=outbuf(out2);
        hideCur(out1); hideCur(out2);
    }
    void open() { update(); }
    void update()
    {
        std::swap(out1, out2); std::swap(ob1, ob2);
        SetConsoleActiveScreenBuffer(out1);
        rdbuf(&ob2); cls(out2);
    }
    void close() { SetConsoleActiveScreenBuffer(GetStdHandle(STD_OUTPUT_HANDLE)); }
    ~Console() { CloseHandle(out1); CloseHandle(out2); }
};

static Console con;

#endif // _CONSOLE
