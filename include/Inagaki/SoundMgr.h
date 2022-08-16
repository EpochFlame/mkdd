#ifndef SOUNDMGR_H
#define SOUNDMGR_H

#include "types.h"
#include "JSystem/JAudio/JAUSoundObject.h"
#include "JSystem/JKernel/JKRHeap.h"

namespace GameAudio
{
    template <typename T>
    class SoundMgr : JAUDopplerSoundObject
    {
        SoundMgr(Vec *, JKRHeap, u8);
        virtual ~SoundMgr();
        virtual void init();
        virtual void setSe(u32 id);
        virtual void loop(u32);
    };

}

#endif