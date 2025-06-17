#ifndef NOTEOUTPUT_H
#define NOTEOUTPUT_H

#include <QAudioFormat>
#include <QAudioOutput>
#include <QElapsedTimer>
#include <QPointer>

#include "audiobuffer.h"
#include "../music/track.h"
#include "../game/maingame.h"
#include "notesamples.h"
#include "../game/maingamestate.h"

class NoteOutput : public QObject
{
Q_OBJECT

public:
    NoteOutput(QPointer<MainGame> game);
    NoteOutput(shared_ptr<AudioBuffer> audioBuffer, shared_ptr<NoteSamples> samples, QPointer<MainGame> game);
    ~NoteOutput();
    bool isPlaying();
    void assignTrack(QPointer<Track> track);
    shared_ptr<AudioBuffer> audioBuffer;
    QMap<MusicInstrument, NoteSamples*> samples;
    void clear();
    void increaseVolume();
    void decreaseVolume();
    void setVolume(int volume);
    void setLatency(int value);

public slots:
    void startOutput();
    void passSamples(NoteSamples* samples);
    void stopOutput();
    void doAudioOutput();
    void feedNotes();

private:
    int samplerate = 0; //for 20ms of time

    int audioFillRate = 10;
    int periodSize;
    char *mixData; // mixed data to write to the sound card

    std::mutex setLatencyMutex;

    bool stopThreads = false;
    qint64 playPosition = 0;
    QList<QPointer<Track>> tracks;
    QPointer<MainGame> game;
    QPointer<QAudioOutput> output;
    QPointer<QIODevice> device;
    QPointer<QThread> outputThread;
    QPointer<QThread> loopThread;
};

#endif // NOTEOUTPUT_H
