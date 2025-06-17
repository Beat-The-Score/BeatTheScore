#ifndef MIDINOTEOUTPUT_H
#define MIDINOTEOUTPUT_H

#include <QPointer>
#include "audiobuffer.h"
#include "notesamples.h"
#include "../game/maingame.h"

class MidiNoteOutput : public QObject
{
    Q_OBJECT
public:
    MidiNoteOutput(shared_ptr<AudioBuffer> audioBuffer, QPointer<MainGame> game);
    void passSamples(NoteSamples* samples);
    void setMusicInstrument(MusicInstrument instrument);

private:
    shared_ptr<AudioBuffer> audioBuffer;
    QMap<MusicInstrument, NoteSamples*> samples;
    QMap<int, unsigned int> pitchIdMap;
    QPointer<MainGame> mainGame;
    MusicInstrument instrumentType = MusicInstrument::Piano;

public slots:
    void noteOn(Note *note);
    void noteOff(Note *note);
};

#endif // MIDINOTEOUTPUT_H
