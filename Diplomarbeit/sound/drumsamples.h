#ifndef DRUMSAMPLES_H
#define DRUMSAMPLES_H

#include <QHash>
#include <memory>
#include "notesamples.h"


using namespace std;

enum DrumInstrument {Kick, Snare, HiHat, LoHat, Crash, Tom1, Tom2, Tom3};

class DrumSamples
{
public:
    DrumSamples();
    shared_ptr<NoteSample> getSample(DrumInstrument instrument);

private:
    QHash<DrumInstrument, shared_ptr<NoteSample>> drumHash;
};

#endif // DRUMSAMPLES_H
