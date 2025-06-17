#ifndef NOTESAMPLES_H
#define NOTESAMPLES_H
#include <memory>
#include <vector>

using namespace std;

enum MusicInstrument {Guitar, EGuitar, BassGuitar, Piano};

struct NoteSample {
    NoteSample()
    {
        //buffer = NULL;
    }
    ~NoteSample()
    {
        if (buffer != NULL)
            delete buffer;
    }

    char * buffer = NULL;
    int bufferSize = 0;
};

#include <QObject>

class NoteSamples : public QObject
{
    Q_OBJECT

public:
    NoteSamples(MusicInstrument instrument);
    void startPitching();
    MusicInstrument getInstrumentType();
    NoteSample* getNote(int pitch);
    static MusicInstrument getInstrumentFromGM(int gMInstrument);

private:
    vector<NoteSample*> notes; // index is the note as specified in the MIDI spec
    MusicInstrument instrument;

signals:
    void pitchChanged();
};

#endif // NOTESAMPLES_H
