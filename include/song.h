#ifndef _SONG
#define _SONG

#include <vector>
#include "header.h"
using namespace std;

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
        vector<int> note;
        void reset()
        {
            now_note = 1;
            can_seen = 1;
            note.clear();
            note.push_back(114514);
        }
        int run()
        {
            int miss=0;
            while(now_note <= note_cnt and !GetNoteState(note[now_note]))
                now_note++,miss++;
            while(can_seen <= note_cnt and GetNoteState(note[can_seen]) != 5)
                can_seen++;
            return miss;
        }
        vector<int> getNotes()
        {
            vector<int> v;
            for(int i=now_note; i<can_seen; i++)
                v.emplace_back(note[i]);
            return v;
        }
    };

    int note_cnt;
    int allTime; // 整体时间
    int trackNum; // 轨道数
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
        cout << "Loading spectrum..." << endl;
        if(fr == nullptr)
        {
            cout << "No spectrum found" << endl;
            Sleep(OneSecond);
            return false;
        }
        
        perfect_tot = 0;
        good_tot = 0;
        miss_tot = 0;

        int type;
        for(int i=1; i<=trackNum; i++) track[i].reset();

        fscanf(fr, "%d %d %d", &note_cnt, &allTime, &type);
        trackNum=type*2; // 转换为轨道数
        for(int i = 1; i <= note_cnt; i++)
        {
            int t,l;
            fscanf(fr, "%d %d", &t, &l);
            track[l].note.push_back(t);
        }
        for(int i=1; i<=trackNum; i++) track[i].note_cnt=track[i].note.size()-1;
        fclose(fr);

        cout << "Spectrum loaded!" << endl;
        Sleep(OneSecond);
        ClearScreen();
        return true;
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
    vector<int> getNotes(int line) { return track[line].getNotes();}

     /**
     * @brief 获取指定行行首note的状态
     * @param line 轨道号
     * @return 0: Miss, 1: Perfect, 2: Great, 3: Bad, 4: Far, 5: Cannot See
    */
    int getStatus(int line)
    {
        int s=GetNoteState(track[line].note[track[line].now_note]);
        if(s<3+(trackNum==4)) ++track[line].now_note;
        return s;
    }

    bool isEnd() { return NowTime()>=allTime;}
};

static Song song;

#endif
