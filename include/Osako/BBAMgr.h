#ifndef BBAMGR_H
#define BBAMGR_H

#include "JSystem/JKernel/JKRHeap.h"

class BBAMgr
{ // Autogenerated
public:
    // Global
    static void create(JKRHeap *);          // 0x8020d618
    BBAMgr(JKRHeap *);                      // 0x8020d678
    void processDHCP();                     // 0x8020d774
    void processAutoIP();                   // 0x8020d984
    void disconnecting(bool);               // 0x8020dbe8
    void startUPnP();                       // 0x8020dce0
    void startMSearch();                    // 0x8020dd0c
    void sendTo(void *, int, const void *); // 0x8020dd34
    void recvFrom(void *, int, void *);     // 0x8020dd60
    void getState();                        // 0x8020dd8c
    void createBasicDevice();               // 0x8020de04
    void loadHttpFile();                    // 0x8020dfe0
    static BBAMgr *mspBBAMgr;               // 0x80416a60
    // Inline/Unused
    // void start();
}; // class BBAMgr
#endif // BBAMGR_H