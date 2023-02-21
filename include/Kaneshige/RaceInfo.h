#ifndef RACEINFO_H
#define RACEINFO_H

#include "types.h"

#include "Kaneshige/KartInfo.h"
#include "Kaneshige/RaceTime.h"

enum ERaceMode
{
    INV_MODE = 0,
    TIME_ATTACK = 0x1,
    GRAND_PRIX = 0x2,
    VERSUS_RACE = 0x3,
    BALLOON_BATTLE = 0x4,
    ROBBERY_BATTLE = 0x5,
    BOMB_BATTLE = 0x6,
    ESCAPE_BATTLE = 0x7,
    AWARD_DEMO = 0x8,
    STAFF_ROLL = 0x9,
};

enum ERaceLevel
{ // unsure of this
    LVL_INV = -1,
    LVL_50CC = 0,
    LVL_100CC = 1,
    LVL_150CC = 2,
    LVL_MIRROR = 3,
};

enum ERaceGpCup
{
    INV_CUP = -1,
    MUSHROOM_CUP = 0,
    FLOWER_CUP = 1,
    STAR_CUP = 2,
    SPECIAL_CUP = 3,
    REVERSE2_CUP = 4,
};

// Kaneshige doesn seem to use s32? refactor if this is the case
class RaceInfo
{
public:
    RaceInfo(); // inlined in release version, inline auto?
    ~RaceInfo();

    static u16 sWaitDemoSelector;
    static ERaceGpCup sAwardDebugCup;

    static int sForceDemoNo;
    static u32 sForceRandomSeed;
    static ERaceLevel sAwardDebugLevel;
    static short sAwardDebugRank;

    static EKartID sAwardDebugKartIDTable[];
    static ECharID sAwardDebugDriver1IDTable[];
    static ECharID sAwardDebugDriver2IDTable[];
    
    void reset();
    void setConsoleTarget(int idx, int p2, bool p3);
    void settingForWaitDemo(bool demoSettingThing);
    void settingForAwardDemo();
    void settingForStaffRoll(bool trueEnding);
    void setRace(ERaceMode RaceMode, int kartCount, int playerCount, int consoleCount, int p5);
    void setRaceLevel(ERaceLevel raceLvl);
    void shuffleRandomSeed();
    void shuffleStartNo();
    void hideConsole(u32 param_2);
    void setKart(int, EKartID, ECharID, KartGamePad *, ECharID, KartGamePad *);

    // Inline Functions
    int getLANLapNumber() const { return mLapNumLAN; }
    int getVSLapNumber() const { return mVsLapNum; }
    int getKartNumber() const { return mKartNum; }
    int getConsoleNumber() const { return mConsoleNum; }
    int getStatusNumber() const { return mStatusNum; }
    ERaceMode getRaceMode() const { return mRaceMode; }
    s32 getItemSlotType() const { return mItemSlotType; }

    bool isLANMode() { return mLanMode; }
    bool isTrueEnding() const { return mTrueEnding; }
    bool isMirror() const { return mMirror; }
    bool isWaitDemo() const  {return mDemoType != 0; }
    bool isDriverLODOn() const  { return (mLOD & 2); };
    bool isHiddingConsole(u32 p1) const {
        return (mHideConsole & 1 << p1) != 0;
    }
    void setAwardKartNo(int kartNo) { mAwardKartNo = kartNo; }
    void setGpCup(ERaceGpCup cup) { mGpCup = cup; }
    void setRandomSeed(u32 value) { mRandomSeed = value; }

    void setRivalKartNo(int rivalNo, int kartNo) {
        JUT_MINMAX_ASSERT(114, 0, rivalNo, 2);
        JUT_MINMAX_ASSERT(115, 0, kartNo, 8)
        mRivalKarts[rivalNo] = kartNo;
    }

    int getConsoleTarget(int cnsNo) const {
        JUT_MINMAX_ASSERT(124, 0, cnsNo, 4);
        return _0x114[cnsNo];
    }

    bool isDemoConsole(int cnsNo) const {
        JUT_MINMAX_ASSERT(129, 0, cnsNo, 4);
        return _0x11c[cnsNo];
    }

    KartInfo *getKartInfo(int kartNo) {
        JUT_MINMAX_ASSERT(173, 0, kartNo, 8);
        return &mKartInfo[kartNo];
    }

//private:
    bool mTinyProcess;
    bool mLanMode;
    bool mTrueEnding;
    u32 mRandomSeed;
    ERaceMode mRaceMode;
    ERaceGpCup mGpCup;
    ERaceLevel mRaceLevel;
    s32 mItemSlotType; // perhaps this is an enum too
    s16 mVsLapNum;
    s16 mLapNumLAN;
    s16 mKartNum;
    s16 mPlayerNum;
    s16 mConsoleNum;
    s16 mStatusNum;
    u16 mLOD;
    s16 mGpStageNo;
    s32 mDemoType;
    bool mMirror;
    // padding bytes
    KartInfo mKartInfo[8];
    s16 mStartPosIndex[8];
    s16 mPointTable[8];
    s16 mRivalKarts[2];

    s16 _0x114[4];
    bool _0x11c[4]; // not sure what these two arrays do, setConsoleTarget sets this so maybe this bool array means isActive and the other the console number / consoleTarget

    s16 mAwardKartNo;
    // padding bytes
    s32 mDemoNextPhase;
    s16 mRank[8]; // stores what rank you finished at previous race, basically the same as startPosIndex
    RaceTime mFinishTime[8];
    RaceTime mLapTimes[8][10];
    s32 _0x298; // mWaitDemoResult, rename this at some point
    u16 mHideConsole;
    s8 _0x29e[0x2e0 - 0x29e]; // unknown
};
// unfortunately i can't enable this yet
// RaceInfo gRaceInfo;

#endif // !RACEINFO_H
