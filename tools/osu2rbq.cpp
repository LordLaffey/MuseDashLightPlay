#include <vector>
#include <cstdio>
#include <cstring>
#include <conio.h>
#include <windows.h>
#include <string.h>
#include <iostream>
using namespace std;

struct node
{
    int start,end,pos;
    node() {}
    node(int s, int e, int p) : start(s), end(e), pos(p) {}
};

int x,y,startTime,type,sound,endTime;
char s[1000];
vector<node> lis;

void change(string str)
{
    FILE* fr = fopen((str+".osu").data(), "r");
    FILE* fw = fopen((str+".rbq").data(), "w");
    while(fscanf(fr, "%s", s))
     if(strcmp(s, "[HitObjects]")==0)
      break;
    
    while(fscanf(fr, "%d,%d,%d,%d,%d,%d%s", &x, &y, &startTime, &type, &sound, &endTime, s)!=EOF)
    {
        if(x==64) x=1;
        if(x==192) x=2;
        if(x==320) x=3;
        if(x==448) x=4;
        lis.emplace_back(startTime, endTime, x);
    }
    fprintf(fw, "%d %d %d\n", lis.size(), lis.back().start+1000, 2);
    for(node e : lis)
    {
        if(e.end)
        fprintf(fw, "%d 2 %d %d\n", e.pos, e.start, e.end);
        else
        fprintf(fw, "%d 1 %d\n", e.pos, e.start);
    }
    fclose(fr); fclose(fw);
}
void singalMode()
{
    string name;
    system("cls");
    puts("input the file's name");
    cin>>name;
    change(name);
    puts("done!");
    _getch();
}
void autoMode()
{
    puts("nothing");
}
int main()
{
    while(1)
    {
        system("cls");
        puts("1. singal file(s)");
        puts("2. auto change(a)");
        puts("3. quit(q)");
        switch(_getch())
        {
        case '1': case 's': case 'S': singalMode(); break;
        case '2': case 'a': case 'A': autoMode(); break;
        case '3': case 'q': case 'Q': case 27: return 0;
        }
    }
    return 0;
}