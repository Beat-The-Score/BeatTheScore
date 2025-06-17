#include "audiobuffer.h"
#include <limits>
#include <string.h>
#include <iostream>

AudioBuffer::AudioBuffer()
{
    sampleList = vector<SampleInfo>();
}

AudioBuffer::~AudioBuffer()
{

}

void AudioBuffer::clear() {
    mutex.lock();
    sampleList.clear();
    mutex.unlock();
}

void AudioBuffer::stopId(unsigned int id)
{
    if (id == 0)
        return;

    mutex.lock();
    for(int i = 0; i < sampleList.size(); i++) {
        SampleInfo tmpSample = sampleList[i];
        if(tmpSample.id == id) {
            if(tmpSample.position + 11025 < tmpSample.duration)
                sampleList[i].duration = tmpSample.position + 11025;
        }
    }
    mutex.unlock();
}

unsigned int AudioBuffer::writeData(NoteSample* note, int offset, int duration, bool getId)
{
    if(note == NULL || note->buffer == NULL)
        return -2;

    mutex.lock();
    SampleInfo sampleInfo;
    if(getId) {
        sampleInfo.id = lastId;
        ++lastId;
        if(lastId == 0)
            ++lastId;
    }

    sampleInfo.noteSample = note;

    if(offset % 2 != 0)
        offset--;
    sampleInfo.position = -offset;

    // don't step out of bounds while mixing
    int smallerDuration = duration > note->bufferSize ? note->bufferSize : duration;
    if(smallerDuration % 2 != 0)
        smallerDuration--;

    sampleInfo.duration = smallerDuration;
    sampleInfo.numOfSamples = numOfSamples + 1;
    sampleList.push_back(sampleInfo);
    mutex.unlock();

    return sampleInfo.id;
}

unsigned int AudioBuffer::writeData(NoteSample* note, bool getId) {
    return writeData(note, 0, note->bufferSize, getId);
}

qint64 AudioBuffer::readData(char * mixedBuffer, qint64 size)
{
    mutex.lock();

    // Clear the buffer
    for(int i = 0; i < size; i++) {
        mixedBuffer[i] = 0;
    }

    // Let's only add up samples that overlap at a single point in time.
    // Also, do mixing in relation to the longest buffer in the vector.
    int longestSample = 0;

    for(int i = 0; i < sampleList.size(); i++) {
        if(sampleList[i].noteSample != NULL && sampleList[i].duration > longestSample) {
            longestSample = sampleList[i].duration < size ? sampleList[i].duration : size;
        }
    }

    // Step through every single point in "time" and mix samples that overlap
    for(int i = 0; i < longestSample; i+=2) {
        short mixedValue = 0;

        for(int j = 0; j < sampleList.size(); j++) {
            if(sampleList[j].noteSample != NULL && sampleList[j].position > 0 &&
                    sampleList[j].position < sampleList[j].noteSample->bufferSize) {
                // Can this part of the sample be mixed?
                int length = (sampleList[j].duration - sampleList[j].position) > size ?
                        size : (sampleList[j].duration - sampleList[j].position);

                if(length > 0) {
                    SampleInfo sample = sampleList[j];

                    if(numOfSamples < sampleList.size()) {
                        sampleList[j].direction = UP;
                    } else if (numOfSamples > sampleList.size()) {
                        sampleList[j].direction = DOWN;
                    }

                    if(sampleList[j].direction == UP) {
                        if(sampleList[j].volumeProgress < (sampleList[j].position)) {
                            sampleList[j].volumeProgress += 1;
                        }
                    } else if(sampleList[j].direction == DOWN) {
                        if(sampleList[j].volumeProgress > 0) {
                            sampleList[j].volumeProgress -= 1;
                        }
                    }

                    if(sampleList[j].volumeProgress > sampleList[j].position+1) {
                        sampleList[j].volumeProgress = sampleList[j].position+1;
                    }

                    qreal countFactor = ((qreal)sampleList[j].volumeProgress / (qreal)(sampleList[j].position+1)) /
                            ((qreal)sampleList[j].numOfSamples);

                    if (sample.position < sample.noteSample->bufferSize) {
                        short tmpVal = ((short*)sample.noteSample->buffer)[sample.position / 2];
                        tmpVal = (short)((qreal)tmpVal * countFactor * getSilencingFactor(j));

                        mixedValue += tmpVal;
                    }
                }
            }

            sampleList[j].position += 2;
        }

        ((short*)mixedBuffer)[i/2] = mixedValue;
    }

    // Remove notes from the vector if they have been mixed completely
    for(int i = sampleList.size()-1; i >= 0; i--) {
        if(i < sampleList.size() && sampleList[i].duration < sampleList[i].position) {
            sampleList.erase(sampleList.begin()+i);
        }
    }

    numOfSamples = sampleList.size();

    mutex.unlock();
    return size;
}

qreal AudioBuffer::getSilencingFactor(int sampleID)
{
    if((qreal)sampleList[sampleID].position < (qreal)sampleList[sampleID].duration * 0.7)
        return 1;

    qreal fact = ((qreal)sampleList[sampleID].duration - (qreal)sampleList[sampleID].position) /
            ((qreal)sampleList[sampleID].duration * 0.3);

    return fact;
}
