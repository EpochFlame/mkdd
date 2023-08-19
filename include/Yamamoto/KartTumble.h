#ifndef KARTTUMBLE_H
#define KARTTUMBLE_H

#include "Sato/ItemObj.h"

class KartTumble
{ // Autogenerated
public:
    KartTumble(){};
    void Init(int);                   // 0x803049f0
    void MakeWanWanTumble(ItemObj *); // 0x80304a44
    void MakeKameTumble(ItemObj *);   // 0x80304cc8
    void MakeStarTumble();            // 0x80304ef8
    void MakeDashTumble();            // 0x80305100
    void DoTumble();                  // 0x803051d4
    void DoPakunTumble();             // 0x80305438
    void DoHanaTumble();              // 0x80305674
    void MakePoiHanaTumble();         // 0x803058ec
    void DoShootCrashCrl();           // 0x80305bd0
    void DoTumbleCrl();               // 0x80305d3c
    void DoAfterTumbleCrl();          // 0x80305d64
private:
    // TODO
};
#endif // KARTTUMBLE_H