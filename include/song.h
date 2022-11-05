#ifndef _SONG
#define _SONG

#include "header.h"
using namespace std;

struct Note {
    int type;       // 1: 短键 2: 长条
    int start,end;
    int len = 0;
    bool hvcheck = false;
    Note() {}
    Note(int t, int s, int e) : type(t), start(s), end(e) {}
};

class Song {
private:
    // 判定偏移
    static const int Prefect = 50;
    static const int Great = 100;
    static const int Bad = 150;
    struct Track
    {
        int note_cnt;
        int now_note, can_seen;
        vector<Note> notes;
        void reset()
        {
            now_note = 1;
            can_seen = 1;
            notes.clear();
            notes.emplace_back();
        }
        int run()
        {
            int miss=0;
            while(now_note <= note_cnt and !GetNoteState(notes[now_note].end))
                now_note++,miss++;
            while(can_seen <= note_cnt and GetNoteState(notes[can_seen].start) != 5)
                can_seen++;
            return miss;
        }
        vector<Note> getNotes()
        {
            vector<Note> v;
            for(int i=now_note; i<can_seen; i++)
                v.emplace_back(notes[i]);
            return v;
        }
    };

    int note_cnt;
    int allTime; // 整体时间
    int trackNum; // 轨道数
    bool isPress;
    Track track[5];

    static int GetNoteState(int time)
    {
        int now_time = NowTime();
        if(now_time-time > Bad) return 0;
        else if(abs(now_time-time) <= Prefect) return 1;
        else if(abs(now_time-time) <= Great) return 2;
        else if(abs(now_time-time) <= Bad) return 3;
        else if(time-now_time <= MDFallTime) return 4;
        else return 5;
    }
public:
    atomic<int> perfect_tot, good_tot, bad_tot, miss_tot;

    bool LoadSpectrum(FILE *fr)
    {
        // 加载文件
        cout << "Loading spectrum..." << endl;
        if(fr == nullptr)
        {
            cout << "No spectrum found" << endl;
            Sleep(OneSecond);
            return false;
        }
        
        // 初始化
        int type,readType;
        perfect_tot = 0;
        good_tot = 0;
        bad_tot = 0;
        miss_tot = 0;
        for(int i=1; i<=trackNum; i++) track[i].reset();

        fscanf(fr, "%d %d %d", &note_cnt, &allTime, &type);
        allTime+=OneSecond;
        trackNum=type*2; // 转换为轨道数
        for(int i = 1; i <= note_cnt; i++)
        {
            int line,type,start,end;
            fscanf(fr, "%d %d %d", &line, &type, &start);
            if(type==2) // 滑条
                fscanf(fr, "%d", &end);
            else
                end=start;
            
            track[line].notes.emplace_back(type, start+OneSecond, end+OneSecond);
        }
        for(int i=1; i<=trackNum; i++) track[i].note_cnt=track[i].notes.size()-1;
        
        fclose(fr);
        cout << "Spectrum loaded!" << endl;
        Sleep(OneSecond);
        ClearScreen();
        return true;
    }

    /**
     * @brief 设置 hold 的初始长度
     * @param flag [0|1] MDMode/FKMode 
    */
    void MakeHolds(bool flag){
        
        const double Speed=!flag?MDSpeed:FourKeySpeed;
        for(int i = 1; i <= trackNum; i++)
        {
            for(auto &v : track[i].notes)
            {
                if(v.type!=2) continue;
                v.len = Speed * (v.end-v.start);
            }
        }
        
    }

    /**
     * @brief 调整当前时间
     * @return 是否有 miss
    */
    
    bool run()
    {
        int miss=0;
        for(int i=1; i<=trackNum; i++)
            miss+=track[i].run();
        miss_tot+=miss;
        return miss!=0;
    }
    
    /**
     * @brief 获取指定轨道应显示的notes
     * @param line 轨道号
    */
    vector<Note> getNotes(int line) { return track[line].getNotes();}

     /**
     * @brief 获取指定行行首note的状态
     * @param line 轨道号
     * @param type [0|1] 键按下/键抬起
     * @return 0: Miss, 1: Perfect, 2: Great, 3: Bad, 4: Far, 5: Cannot See
    */

    int getStatus(int line,int type){
        
        Note &note = track[line].notes[track[line].now_note];
        int lstpress=isPress;
        
        isPress=(type==1);
        if(note.type==1&&type==1)
        {
            int state = GetNoteState(note.start);
            if(state<=2+(trackNum==4)) track[line].now_note++;
            return GetNoteState(note.start);
        }
        else if(note.type==2)
        {
            if(type==1)
            {
                int s=GetNoteState(note.start);
                if(note.hvcheck)
                {
                    if(NowTime()>note.end)
                        track[line].now_note++,s=1;
                    else s=4;
                }
                else if(!note.hvcheck)
                {
                    if(!lstpress&&s<=2+(trackNum==4))
                        note.hvcheck=true;
                    else s=4;
                }
                return s;
            }
            else
            {
                int e=GetNoteState(note.end);
                if(note.hvcheck)
                {
                    track[line].now_note++;
                    if(e>=1&&e<=2) return e;
                    else return 0;
                }
            }
        }
        
        return 5;
        
    }

    bool isEnd() { return NowTime()>=allTime;}
    int GetAllTime() { return allTime;}
};

static Song song;

#endif
