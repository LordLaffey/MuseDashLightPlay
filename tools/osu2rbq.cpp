#include<vector>
#include<cstdio>
#include<cstring>
using namespace std;

int x,y,tim,type;
char s[1000];
vector< pair<int,int> > lis;

int main()
{
    printf("input the osu file's name:");
    scanf("%s", s);
    FILE* fr = fopen(s, "r");
    printf("input the rbq file's name:");
    scanf("%s", s);
    FILE* fw = fopen(s, "w");
    puts("osu to rbq ing...");

    while(fscanf(fr, "%s", s))
     if(strcmp(s, "[HitObjects]")==0)
      break;
    
    while(fscanf(fr, "%d,%d,%d,%d,%[^\n]", &x, &y, &tim, &type, s)!=EOF)
    {
        if(x==64) x=1;
        if(x==192) x=2;
        if(x==320) x=3;
        if(x==448) x=4;
        lis.emplace_back(tim, x);
    }
    
    fprintf(fw, "%d %d %d\n", lis.size(), lis.back().first+1000, 2);
    for(pair<int,int> e : lis)
    fprintf(fw, "%d %d\n", e.first, e.second);
    fclose(fr); fclose(fw);
    puts("done!");
    return 0;
}
