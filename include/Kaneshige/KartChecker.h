#ifndef KARTCHECKER_H
#define KARTCHECKER_H

#include "types.h"

#include "JSystem/JGeometry.h"
#include "JSystem/JUtility/JUTDbPrint.h"
#include "JSystem/JUtility/JUTDbg.h"
#include "Kaneshige/JugemPoint.h"
#include "Kaneshige/Course/Course.h"
#include "Kaneshige/KartInfo.h"
#include "Kaneshige/RaceTime.h"
#include "Sato/ItemObj.h"

#define MAX_FRAME 2147483

#define validUD(UD) \
    (UD >= 0.0f && UD <= 1.0f);

class KartChecker { // TODO: organise this class better
public:
    KartChecker(int, KartInfo *, int, int);

    void clrPass(int sectoridx) {
        int index = sectoridx / 32;
        int bitIndex = sectoridx % 32;
        mPassedSectors[index] &= ~(1 << bitIndex);
    }

    int getRank() const { return mRank; }
    RaceTime *getBestLapTime();
    const RaceTime &getTotalTime() { return mTotalTime; }
    bool isBestTotalTimeRenewal(int);
    bool isBestLapTimeRenewal();
    bool isCurrentLapTimeRenewal();
    bool isLapRenewal() const { return mLapRenewal; }
    bool isFinalLap() const { return mLap == mMaxLap - 1; }
    bool isFinalLapRenewal() const;
    bool isGoal() const { return mRaceEnd; }

    bool isPass(int sectoridx)  {
#line 129
        int index = sectoridx / 32;
        int bitIndex = sectoridx % 32;        
        JUT_MINMAX_ASSERT(0, index, mNumBitfields);
        return (mPassedSectors[index] & (1 << bitIndex)) != false;
    }

    bool isPassAll(int sectorCnt);

    bool isRabbitWinner() const { return (mRabbitWinFrame <= 0); }

    bool isReverse();

    // https://decomp.me/scratch/RWx4a
    void printPass(int x, int y) {
        for (int i = 0; i < mNumBitfields; i++) {
            JUTReport(x, (y + 16) + (i * 16), "[%d]:%08X", i, mPassedSectors[i]);
        }
    }

    const RaceTime &getLapTime(int no) {
#line 206
        JUT_MINMAX_ASSERT(0, no, mMaxLap);
        return mLapTimes[no];
    }

    KartGamePad * getDriverPad(int driverNo) const {
#line 220
        JUT_MINMAX_ASSERT(0, driverNo, 2);
        return mKartGamePads[driverNo];
    }

    void setGoal() {
        mIsInRace = false;
        mRaceEnd = true;
    }

    // might be a reference
    const RaceTime &getDeathTime() { return mDeathTime; };
    int getBalloonNumber() const { return mBalloonNum; };
    bool isRankAvailable() const { return mRank != 0; };
    int getBombPoint() const { return mBombPoint; };
    const RaceTime &getMarkTime() { return mMarkTime; };
    f32 getTotalUnitDist() const { return mRaceProgression; };
    // cmpw was signed right?
    int getGoalFrame() const { return mGoalFrame; };

    void setForceGoal();

    void setGoalTime() {
        mTotalTime = mBestLapTimes[mMaxLap - 1];
        mGoalFrame = mCurFrame;
    }

    void setLapTime();

    void setLapChecking() { mRaceFlags |= 1; }
    void setBalloonCtrl() { mRaceFlags |= 2; }
    void setBombCtrl()  { mRaceFlags |= 4; }
    void setRabbitCtrl() { mRaceFlags |= 8; }
    void setDemoRank() { mRaceFlags |= 16; }
    void setDead() { mBattleFlags |= 4; }
    void setRank(int rank) { mRank = rank; }
    inline bool setPass(int index); // ??? function is weak yet in cpp file itself
    void clrRank() { mRank = 0; }
    void resumeRabbitTimer() { mBattleFlags &= 0xfffe; }
    bool tstLapChecking() const { return mRaceFlags & 1; }
    bool tstBalloonCtrl() const { return mRaceFlags & 2; }
    bool tstBombCtrl() const { return mRaceFlags & 4; }
    bool tstRabbitCtrl() const { return mRaceFlags & 8; }
    bool tstDemoRank() const { return mRaceFlags & 16; }
    bool tstFixMiniPoint() const { return mBattleFlags & 2; }
    bool tstDead() const { return mBattleFlags & 4; }
    bool tstStillRabbitTimer() const { return mBattleFlags & 1; }
    bool isMaxTotalTime() const { return !mTotalTime.isAvailable(); }
    bool isDead() const { return tstDead(); }
    bool isBombPointFull() const { return mBombPoint >= sBombPointFull; }
    
    static bool isInsideSector(f32 unitDist) { return (unitDist >= 0.0f && unitDist < 1.0f); }
    static int getWinBombPointForMenu(int p1) {
        if (p1 <= 2)
            return sBombPointFullS;
        return sBombPointFullL;
    }

    void incLap() {
        if (mLap < mMaxLap)
            mLap++;
    }

    bool incBalloon();
    bool decBalloon();


    void incTime();
    bool incMyBombPoint(int, int);
    static bool incYourBombPoint(int idx, int pnt, int increment);

    bool isUDValid();

    void reset();
    void setPlayerKartColor(KartInfo *);
    void createGamePad(KartInfo *);
    void clrCheckPointIndex();

    static Course::Sector *searchCurrentSector(f32 *, JGeometry::TVec3<f32> const &, Course::Sector *sector, int);
    void checkKart();
    void checkKartLap();
    void checkLap(bool);

    int getRobberyItemNumber();
    bool releaseRabbitMark();
    bool isRabbit() const;
    void calcRabbitTime();

    enum EBombEvent {
        EVENT_1 = 1,
        EVENT_2 = 2,
        EVENT_3 = 3
    };

    void setBombEvent(EBombEvent, ItemObj *);

    static int sPlayerKartColorTable[];
    static short sBalForbiddenTime;

    static short sBombPointDirect;   // 1
    static short sBombPointSpin;     // 1
    static short sBombPointIndirect; // 1
    static short sBombPointAttacked; // -1
    static short sBombPointFull;     // 4
    static short sBombPointFullS;    // 3
    static short sBombPointFullL;    // 4

    static short sBombPointCrushOneself;

    // private: // i'm not really sure how else KartChkUsrPage got access to this
    u16 mRaceFlags;
    s16 mTargetKartNo;
    int mNumSectors;
    int mNumBitfields;
    int mMaxLap;
    int mBestLapIdx;
    RaceTime *mLapTimes;
    RaceTime *mBestLapTimes;
    int mPlayerKartColor;
    KartGamePad *mKartGamePads[2];
    bool mLapRenewal;
    bool mRaceEnd;
    u8 _0x2a; // only seems to get set in the constructor
    int mLap;
    f32 mSectorProgression;
    int mWarpState;
    int mGeneration;
    int mSectorIdx;
    Course::Sector *mSector1; // TODO: figure out difference between these, sector 2 seems to get used all the time
    Course::Sector *mSector2;
    f32 mLapProgression;
    f32 mPrevLapProgression;
    f32 mLapProgression2; // might be max Lap Progression
    f32 mRaceProgression;
    u32 *mPassedSectors; // array of what bitfields have been passed(1 = passed, 0 = not passed)
    JGeometry::TVec3<f32> mPos;
    JGeometry::TVec3<f32> mPrevPos;
    JugemPoint *mJugemPoint;
    bool mIsInRace;
    int mCurFrame;
    int mGoalFrame;
    RaceTime mTotalTime;
    int mRank;
    u16 mBattleFlags;
    s16 mBalForbiddenTime;
    s16 mBalloonNum;
    RaceTime mDeathTime;
    RaceTime mMarkTime;
    s8 mBombPointTable[10];
    s16 mBombPoint;
    s16 mRabbitWinFrame;
    int mDemoPoint;
    // these only get set in the constructor?
    JGeometry::TVec3<f32> _0xb0;
    int _0xbc;
};

class LapChecker
{
public:
    LapChecker();
    void reset();
    void start(Course::Sector *sector);
    void calc(const JGeometry::TVec3<f32> &);
    bool isUDValid();

private:
    Course::Sector *mSector;
    float mSectorDist;
    float mLapUnitDist;
};

#endif // !KARTCHECKER_H
