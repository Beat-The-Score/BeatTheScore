#ifndef AUDIOBUFFER_H
#define AUDIOBUFFER_H

#include <QIODevice>
#include <mutex>
#include <vector>
#include "notesamples.h"
#include "drumsamples.h"

using namespace std;

enum VolumeDirection { UP, DOWN };

struct SampleInfo
{
    // sample information
    unsigned int id = 0;
    NoteSample *noteSample = NULL;
    int position = 0;
    int duration = 0;

    // clipping
    VolumeDirection direction = UP;
    int volumeProgress = 0;
    int numOfSamples = 0;
};

class AudioBuffer
{

private:
    vector<SampleInfo> sampleList;
    std::mutex mutex;
    unsigned int lastId = 0;
    int mixBufferCount = 0;
    int numOfSamples = 0;

    qreal getSilencingFactor(int sampleID);

public:
    AudioBuffer();
    ~AudioBuffer();
    qint64 readData(char * mixedBuffer, qint64 size);
    unsigned int writeData(NoteSample* note, bool getId);//
    unsigned int writeData(NoteSample* note, int offset, int duration, bool getId);
    void stopId(unsigned int id);
    void clear();
};

#endif // AUDIOBUFFER_H
