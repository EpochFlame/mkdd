#ifndef EFFECTSCREEN_H
#define EFFECTSCREEN_H

class EffectScreenMgr
{
public:
    EffectScreenMgr();
    static void createMgr() {
        ThisMgr = new EffectScreenMgr();
    }

    static EffectScreenMgr * getThis() {
        return ThisMgr;
    }

    static EffectScreenMgr *ThisMgr;
};

inline void CreateEfctScreenMgr() {
    EffectScreenMgr::createMgr();
}

inline EffectScreenMgr * GetEfctScreenMgr() {
    return EffectScreenMgr::getThis();
}

#endif