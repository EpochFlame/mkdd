#ifndef MOVIEPLAYER_H
#define MOVIEPLAYER_H

#include "kartLocale.h"
#include "JSystem/JKernel.h"

#include "Kaneshige/THP/THPInfo.h"

class MoviePlayer : public JKRDisposer
{ // Autogenerated
public:
    // Global
    MoviePlayer(VideoMode, VideoFrameMode, JKRHeap *); // 0x801d81e4
    virtual ~MoviePlayer();                            // 0x801d8314
    static void create(JKRHeap *);                     // 0x801d8178
    void reset();                                      // 0x801d838c
    void calc();                                       // 0x801d840c
    void draw();                                       // 0x801d842c
    void drawDone();                                   // 0x801d8498
    void quit();                                       // 0x801d84b8
    void play();                                       // 0x801d84f4
    void replay();                                     // 0x801d8534
    void pause();                                      // 0x801d8560
    void audioFadeOut(int);                            // 0x801d858c
    int getFrameNumber();                             // 0x801d85bc

    static MoviePlayer *getPlayer() { return sPlayer; }

    static const char *cMovieFileNameTable;   // 0x804147d0
    static const char *cMovie50FileNameTable; // 0x804147d4
    static MoviePlayer *sPlayer;              // 0x80416848
    static int sMovieSelector;                // 0x8041684c

private:
    JKRHeap *mHeap;
    VideoMode mVideoMode;
    VideoFrameMode mFrameMode;
    int mDrawFrame;
    bool mIsDonePlaying;
    u16 _2a;
    u8 *mBuffer;
    u8 *mBuf_30;
    THPVideoInfo mVideoInfo;
    u32 _40;
    bool mIsPlaying;
};
#endif // MOVIEPLAYER_H