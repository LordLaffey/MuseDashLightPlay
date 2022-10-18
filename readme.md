~~大饼~~

# Muse Dash Light Play

Muse Dash Light Play - 一款基于控制台的简约版音游。

版本：v1.20

界面(MuseDash Mode)：

```
Perfect         Bad             Miss
7777            7777            7777
-----------------------------------------------
()    <      O===========O
() <     <
===============================================
```

界面(FourKey Mode):

```
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
```

使用方法：编译运行 `main.exe`

> 如果在游玩时出现程序异常结束的问题，请检查编译选项中是否开启了 `optimize` 优化，如果有，请取消。
>
> 4Key 模式的谱子可以使用 recorder 录制或使用 tools 中的 osu2rbq 将 osu 谱面转为 MDLP 谱面。
>
> rbqv2.0 文件更改了 note 的存储格式，旧版本的 MDLP 无法兼容，请使用 MDLP v1.20 及以上版本，recorder 只支持 rbqv2.0 文件的单 note，如果想要使用 hold 键，敬请期待 MDLP NotesMaker，或者使用 osu2rbq。

欢迎游玩，提出建议，或做出贡献（提交 Issues 和 PRs），万分感激 >w<。

谱面可以从 [MDLPSongs: MDLP 谱面社区](https://github.com/qingchenling/MDLPSongs) 获取。

## 特性

- **体积轻小** - 使用不到 20KB 的 C++ 代码完成，可以随时编译运行。基于控制台输出，可以兼容大多数环境。
- **易于上手** - 采用经典的音游判定方式。
- **可扩展性强** - ~~因为什么功能都实现，所以什么功能都有可能实现~~。

## 更新日志

### main:
v0.04 将 settings 独立进 `main.cpp` 中。

v0.05 增加谱面列表，支持自定义键位与键数。

v1.00 **增加了 4Key 模式**。

v1.01 隐藏了光标。

v1.10 成功阻止了程序的电摇。

v1.20 增加了 hold 键。

### player

v0.06 合并了线程

v1.00 引入了双缓冲。

v1.01 优化了判定。

v1.02 增加了实时响应。

v1.03 调整了判定时间。

v1.04 增加了 combo 显示。

v1.05 增加了 hold 键。

### recorder:

v1.11 支持 record 多个谱面。

v1.12 修复了 record 谱面后没有立即加载的 bug。

v1.13 兼容了 4Key 录制。

v1.14 兼容了 rbqv2.0。

## TODO

最后更新于 2022-10-18 16:40

- [x] key mode

- [x] 筛选界面（musedash/xkey）

- [ ] 完善谱面信息

- [x] UI优化

- [ ] 谱面排行榜与评论区

- [ ] 谱面预览

- [ ] 谱面难度系统与对应筛选系统

- [x] 判定实时响应

- [x] hold 键
