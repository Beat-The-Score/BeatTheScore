#include "drumsamples.h"
#include <QFile>
#include <QHash>

using namespace std;

DrumSamples::DrumSamples()
{

    NoteSample * dSample = new NoteSample();

    QFile f("wav/drum/kick.wav");
    QByteArray bArray = f.readAll();
    f.close();
    dSample->buffer = bArray.data();
    dSample->bufferSize = bArray.size();
    //dSample->note = MusicNote::C;
    shared_ptr<NoteSample> kickSample = shared_ptr<NoteSample>(dSample);

    drumHash.insert(DrumInstrument::Kick, kickSample);
}


shared_ptr<NoteSample> DrumSamples::getSample(DrumInstrument instrument) {
    return drumHash.take(instrument);
}
