#ifndef RACEMGR_H
#define RACEMGR_H

#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "Kawano/StaffRoll2D.h"
#include "Kaneshige/Course/Course.h"
#include "Kaneshige/RaceBGMPlayer.h"
#include "Kaneshige/RaceDirector.h"
#include "Kaneshige/RaceInfo.h"
#include "Kaneshige/RaceTime.h"
#include "Kaneshige/KartChecker.h"
#include "Kaneshige/KartLoader.h"
#include "Kaneshige/RaceDrawer.h"
#include "Kaneshige/SysDebug.h"
#include "Osako/Award2D.h"
#include "Yamamoto/kartCtrl.h"
#include "kartEnums.h"
#include "types.h"

void PrintRaceHeap(u32, JKRHeap *);

class RaceMgr : JKRDisposer {
public:
    RaceMgr(RaceInfo *);
    virtual ~RaceMgr();

    struct EventInfo {
        s16 id;
        const char *jpName;
        const char *engName;
    };

    static EventInfo sEventTable[];

    static short sMyStartPointID;

    static RaceMgr *sRaceManager;
    static short sForceTotalLapNum;
    static short sDispFrameCounter;


    static RaceMgr *getManager() { return sRaceManager; }

    class Console {
    public:
        Console();
        void changeTargetNo(int, bool);
        // Unused/Inline
        void clearZBuffer();
        bool isZoom();
        // Inline
        void setDraw();         // 0x801b033c
        void setConsoleNo(int); // 0x801b034c
        void clrDraw();         // 0x801b0354
        bool isDraw() const;    // 0x801b0364
        void clrJugemZClr();    // 0x801b03c4
        void setJugemZClr();    // 0x801b03d4
        bool isNoStat() const;  // 0x801b03e4
        bool isValid() const;   // 0x801b03f0

    private:
        int _0x0;
        u8 _0x4;
        int _0x8;
        ushort _0xc;
    };

    void updateBestTime();            // 0x801ad064
    void setRandomSeed();             // 0x801ad164
    void editRaceInfoForDebug();      // 0x801ad1bc
    void createConsole();             // 0x801ad210
    void resetConsole();              // 0x801ad2e8
    bool hideConsole(u32);            // 0x801ad368
    void createModel();               // 0x801ad3c0
    void createKartModel();           // 0x801ad4ac
    void createCourseModel();         // 0x801ad578
    void createObjectModel();         // 0x801ad604
    void createItemModel();           // 0x801ad694
    void createEffectModel();         // 0x801ad724
    void createLight();               // 0x801ad7d0
    void resetRace();                 // 0x801ad9c4
    void resetRaceForResetEvent();    // 0x801ada58
    void resetRaceForRestartEvent();  // 0x801ada98
    void resetRaceForReplayEvent();   // 0x801adb04
    void resetRaceCommon();           // 0x801adb48
    int getCurrentConsoleNumber();   // 0x801add38
    void drawRace();                  // 0x801add84
    void checkKart();                 // 0x801ade64
    void checkRank();                 // 0x801ae00c
    void checkRankForBalloonBattle(); // 0x801ae0dc
    void checkRankForRobberyBattle(); // 0x801ae238
    void checkRankForBombBattle();    // 0x801ae324
    void checkRankForEscapeBattle();  // 0x801ae4b8
    void checkRankForAwardDemo();     // 0x801ae5a8
    void checkRankForRace();          // 0x801ae600
    void setRaceResult();             // 0x801ae774
    void calcRace();                  // 0x801ae870
    void frameWork();                 // 0x801ae9d0
    void updateRace();                // 0x801aeac0
    bool isRaceModeMiniGame();        // 0x801aef9c

    int getProcLevel(); // 0x801af654
    // Inline Functions
    // RaceInfo related 
    bool isLANMode() const { return mRaceInfo->isLANMode();  }
    bool isMirror() const { return mRaceInfo->isMirror(); }
    bool isWaitDemoMode() const { return mRaceInfo->isWaitDemo(); };
    ERaceMode getRaceMode() const { return mRaceInfo->getRaceMode(); }
    int getConsoleNumber() const { return mRaceInfo->getConsoleNumber(); }
    int getKartNumber() const { return mRaceInfo->getKartNumber(); }
    int getTotalLapNumber() const { return mTotalLapNumber; }
    
    KartInfo *getKartInfo(int index) {
        JUT_RANGE_ASSERT(170, 0, index, 8);
        return &mRaceInfo->mKartInfo[index];
        // return mRaceInfo->getKartInfo(index);
    }

    bool checkRaceEnd() { return mRaceDirector->checkRaceEnd(); }
    ERacePhase getRacePhase() { return mRaceDirector->getRacePhase(); }
    KartDrawer *getKartDrawer(int idx) { return mRaceDrawer->getKartDrawer(idx); };
    int getCameraNumber() const { return getConsoleNumber(); }


    bool isCrsDemoMode() {
        return getRacePhase() == PHASE_CRS_DEMO;
    }
    // RaceMgr get/sets
    Course *getCourse() const { return mCourse;  }    
    const RaceTime &getBestLapTime() { return mBestLapTime; }
    bool isRaceModeVs() const { return getRaceMode() == VERSUS_RACE; }
    bool isRaceModeTA() const { return getRaceMode() == TIME_ATTACK; }
    bool isStaffRoll() { return getRaceMode() == STAFF_ROLL; } // pls don't tell me it's stored as local variable
    bool isAwardDemoMode() {return getRaceMode() == AWARD_DEMO;}
    u16 isActiveAreaLight() const { return mAreaLight & 1; }

    void activeAreaLight() {
        mAreaLight |= 1;
    }

    KartLoader *getKartLoader(int index) const {
        JUT_RANGE_ASSERT(257, 0, index, 8);
        return mKartLoader[index];
    }

    KartCam *getCamera(int cameraNo) const {
        JUT_RANGE_ASSERT(262, 0, cameraNo, 4);
        return KartCtrl::getKartCtrl()->getKartCam(cameraNo);
    }

    KartChecker *getKartChecker(int index) const {
        JUT_RANGE_ASSERT(267, 0, index, 8);
        return mKartChecker[index];
    }

    const RaceTime &getBestTotalTime(int recID) {
        JUT_RANGE_ASSERT(328, 0, recID, 5);
        return mBestTotalTimes[recID];
    }

    private: 
    RaceDirector *mRaceDirector;
    RaceDrawer *mRaceDrawer;
    u16 mAreaLight;
    u8 _0x22; // raceNum?
    u8 _0x23; // probably padding
    u32 mFrame;
    JKRHeap *mRaceHeap;
    u16 mReplayMode;
    s16 mTotalLapNumber;
    s32 _0x30;
    bool mAbleStart;
    RaceInfo *mRaceInfo;
    RaceBGMPlayer *mRaceBGMPlayer;
    Console *mConsole;
    Course *mCourse;
    KartChecker *mKartChecker[8];
    KartLoader *mKartLoader[8];
    Award2D * mAward2D;
    StaffRoll2D *mStaffRoll2D;
    void * mAwardArc;
    RaceTime mBestLapTime;
    RaceTime mBestTotalTimes[5];
    s16 mEvents;

    // these values store the amount of time/frames it took to execute a certain set of functions
    // 0x0 GX Issue
    // 0x100 Calc
    // 0x200 update
    // 0x300 ViewCalc
    s16 mProctime1;
    s16 mProctime2;
    s16 mProctime3;
    s16 mProctime4;
};

class RaceUsrPage : public SysDbUsrPage {
public:
    RaceUsrPage(RaceInfo *raceMgr)
    {
        mRaceMgr = raceMgr;
    }
    virtual ~RaceUsrPage() {};
    virtual void draw();

    RaceInfo *mRaceMgr;
};

inline RaceMgr *RCMGetManager() { 
    return RaceMgr::getManager(); }

inline Course *RCMGetCourse() { // might be inline off, auto?(Kameda)
    return RaceMgr::getManager()->getCourse();
}

inline KartCam *RCMGetCamera(int index) {
    return RaceMgr::getManager()->getCamera(index);
}

inline KartChecker *RCMGetKartChecker(int index) {
    return RaceMgr::getManager()->getKartChecker(index);
}

inline KartLoader *RCMGetKartLoader(int index) {
    return RaceMgr::getManager()->getKartLoader(index);
}

#endif // RACEMGR_H
