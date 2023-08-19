#ifndef KARTPIPE_H
#define KARTPIPE_H

class KartPipe
{ // Autogenerated
public:
    KartPipe() {}

    void Init(int);        // 0x8031228c
    void MakePipe();       // 0x803122d4
    void DoKeep();         // 0x80312494
    void DoCatch();        // 0x8031250c
    void SetPose();        // 0x8031256c
    void DoShoot();        // 0x803127e0
    void DoCheckOutView(); // 0x80312850
    void DoCheckEnd();     // 0x80312914
    void DoPipeCrl();      // 0x803129a4
    void DoAfterPipeCrl(); // 0x80312b8c
private:
    // TODO
};
#endif // KARTPIPE_H