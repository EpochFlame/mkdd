#include <dolphin/gx.h>
#include "JSystem/JFramework/JFWDisplay.h"
#include "JSystem/JUtility/JUTVideo.h"
#include "Osako/system.h"
#include "Kaneshige/ScrnShot.h"
#include "Kaneshige/SysDebug.h"

ScrnShot * ScrnShot::sScrnShot;

// This is an attempt to reconstuct the code without a source
void ScrnShot::createScrnShot(int p1, int p2, _GXTexFmt p3) {
    sScrnShot = new(SysDebug::getManager()->getDebugHeap(), 0) ScrnShot(p1, p2, p3);
}

ScrnShot::ScrnShot(int p1, int p2, _GXTexFmt p3) {
    _0 = System::getDisplay()->getEfbWidth();
    _4 = System::getDisplay()->getEfbHeight();
}

int ScrnShot::capture() {
    if(mMakeScrnShot) {
        GXSetTexCopySrc(0, 0, _0, _4);
        GXSetTexCopyDst(_0, _4, GX_TF_RGBA8, GX_FALSE);
        GXCopyTex(_C, GX_FALSE);
        GXPixModeSync();
        GXDrawDone();
        mMakeScrnShot = false;
    }

    return 0;
}
