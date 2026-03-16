#ifndef GAMMA_TRACK_INFO_HH
#define GAMMA_TRACK_INFO_HH

#include "G4VUserTrackInformation.hh"

class GammaTrackInfo : public G4VUserTrackInformation
{
public:
    GammaTrackInfo() = default;
    explicit GammaTrackInfo(int id) : _historyID(id) {}
    virtual ~GammaTrackInfo() = default;

    void SetHistoryID(int id) { _historyID = id; }
    int GetHistoryID() const { return _historyID; }

    void SetSplitDone(bool v) { _splitDone = v; }
    bool GetSplitDone() const { return _splitDone; }

    void SetHasComptonInK3(bool v) { _hasComptonInK3 = v; }
    bool GetHasComptonInK3() const { return _hasComptonInK3; }

    void SetCrossedK2Plane(bool v) { _crossedK2Plane = v; }
    bool GetCrossedK2Plane() const { return _crossedK2Plane; }

private:
    int _historyID = -1;
    bool _splitDone = false;
    bool _hasComptonInK3 = false;
    bool _crossedK2Plane = false;
};

#endif