#include <QBuffer>
#include <QFile>
#include <QSound>
#include <QEventLoop>
#include <QTimer>
#include <QTime>
#include <QThread>
#include <memory>
#include <iostream>
#include "noteoutput.h"
#include "audiobuffer.h"
#include "../music/note.h"
#include "notesamples.h"
#include "SoundTouch.h"

using namespace std;
using namespace soundtouch;

NoteOutput::NoteOutput(QPointer<MainGame> game)
{
    setParent(game);
    this->game = game;
    QAudioFormat format;
    format.setSampleSize(16);
    format.setSampleRate(44100);
    format.setChannelCount(1);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    //output->setBufferSize(1024);

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if (!info.isFormatSupported(format)) {
        cout << "format sadly unsupported, trying to find nearest format" << endl;
        format = info.nearestFormat(format);
    }
    this->samplerate = format.sampleRate() / 1000;
    this->output = new QAudioOutput(format);
    this->audioBuffer = shared_ptr<AudioBuffer>(new AudioBuffer());

}

NoteOutput::~NoteOutput()
{
    delete output;
    delete device;
    delete loopThread;
    delete outputThread;
}

void NoteOutput::passSamples(NoteSamples* samples)
{
    /*MusicInstrument instrument = MusicInstrument::Piano;
    this->samples = shared_ptr<NoteSamples>(new NoteSamples(instrument));*/
    this->samples[samples->getInstrumentType()] = samples;
    //this->samples = samples;
}

bool NoteOutput::isPlaying()
{
    return output->state() == QAudio::ActiveState;
}

void NoteOutput::increaseVolume()
{
    qreal vol = output->volume();
    if(vol < 1.0) {
        output->setVolume(vol+0.1);
    }
}

void NoteOutput::decreaseVolume()
{
    qreal vol = output->volume();
    if(vol > 0.0) {
        output->setVolume(vol-0.1);
    }
}

void NoteOutput::setVolume(int volume)
{
    output->setVolume(volume / 100.0);
}

void NoteOutput::assignTrack(QPointer<Track> track) {
    Q_ASSERT(!track.isNull());
    this->tracks.push_back(track);
    //this->notes = track->notes;
}

void NoteOutput::clear()
{
    tracks.clear();
    audioBuffer->clear();
    playPosition = 0;
}

void NoteOutput::feedNotes()
{
    int startIndex;
    QElapsedTimer timer;
    timer.start();
    while(!stopThreads) {
        State localState = game->getState();
        //qDebug() << "game time: " << game->getGameTime();
        //qDebug() << "state : " << localState;
        if(localState == PLAYING && playPosition != game->getGameTime()) {
            playPosition = this->game->getGameTime();
            for (int j = 0; j < tracks.size(); j++) {
                if(!tracks[j]->isPercussion) {
                    MusicInstrument instrumentType = NoteSamples::getInstrumentFromGM(tracks[j]->gMInstrument);
                    //startIndex = notes[j]->getStartIndex(game->getSectionIndex());
                    startIndex = 0;

                    QList<QPointer<Note>> noteLine = tracks[j]->notes;
                    for (int i = startIndex; i < noteLine.length(); i++) {
                        if(!noteLine[i].isNull() && noteLine[i]->getTime() >= playPosition && noteLine[i]->getTime() <= playPosition + 20) {

                            /* Safety net for checking whether notes have been added to the buffer beforehand
                               should not be required at the moment since the first track in here is always an input track.
                               Though, in the future, to-be-added samples should be queued up and written afterwards. */
                            if (game->getGameMode() == GameMode::TRAINING) {
                                if(tracks[j]->isInputTrack && !noteLine[i]->isSetSkip() &&
                                        localState == State::PLAYING){
                                    game->awaitNote(noteLine[i]->getTime());
                                }
                            }

                            if(localState == State::PLAYING) {
                                if (!tracks[j]->isInputTrack || game->getGameMode() == GameMode::REPLAY) {
                                    if (samples[instrumentType] != NULL)
                                    {
                                        NoteSample *tmpNote =
                                                samples[instrumentType]->getNote(noteLine[i]->getPitch());
                                        audioBuffer->writeData(tmpNote,
                                                               (playPosition - noteLine[i]->getTime()) * samplerate,
                                                               noteLine[i]->getDuration() * samplerate * 4,
                                                               false);
                                    }
                                }
                            }
                        }
                    }
                }
            }

            if(localState == PLAYING &&
                    game->getLengthOfSelectedSong() > 0 && game->getLengthOfSelectedSong() < game->getGameTime()) {
                game->setState(RESULT); // Until we have something to orchestrate the game
                //timeToLastNote = 0;
            }
        }

        int sleepTime = (20 - (timer.elapsed() % 20));

        if(sleepTime > 20 || sleepTime < 0) {
            sleepTime = 19;
        }

        QThread::msleep(sleepTime);
    }
}

void NoteOutput::setLatency(int value)
{
    setLatencyMutex.lock();

    audioFillRate = value;
    periodSize = (int)((qreal)(output->periodSize() / ((qreal)20 / (qreal)audioFillRate)));

    if(mixData)
        delete [] mixData;

    mixData = new char[periodSize]();

    setLatencyMutex.unlock();
}

void NoteOutput::doAudioOutput()
{
    QElapsedTimer timer;
    timer.start();
    while(!stopThreads) {
        setLatencyMutex.lock();

        int chunks = output->bytesFree()/periodSize;
        qint64 rSize = 0;
        if(chunks) {
            rSize = audioBuffer->readData(mixData, periodSize);
            device->write(mixData, rSize);
        }

        int sleepTime = (audioFillRate - (timer.elapsed() % audioFillRate));

        if(sleepTime > audioFillRate || sleepTime < 0) {
            sleepTime = (audioFillRate - 1);
        }

        setLatencyMutex.unlock();
        QThread::msleep(sleepTime);
    }
}

void NoteOutput::startOutput()
{
    setLatencyMutex.lock();

    if(isPlaying()) {
        stopOutput();
    }
    stopThreads = false;
    this->loopThread = new QThread();
    this->outputThread = new QThread();
    connect(outputThread, SIGNAL(started()), SLOT(doAudioOutput()), Qt::DirectConnection);
    connect(loopThread, SIGNAL(started()), SLOT(feedNotes()), Qt::DirectConnection);
    device = output->start();
    device->open(QIODevice::ReadWrite);

    periodSize = (output->periodSize() / (20 / audioFillRate));
    mixData = new char[periodSize]();

    outputThread->start(QThread::TimeCriticalPriority);
    loopThread->start(QThread::TimeCriticalPriority);

    setLatencyMutex.unlock();
}

void NoteOutput::stopOutput()
{
    stopThreads = true;
    if(!outputThread.isNull()) {
        outputThread->terminate();
        outputThread->wait();
    }

    if(!loopThread.isNull()) {
        loopThread->terminate();
        loopThread->wait();
    }

    if(!output.isNull())
        output->stop();

    if(!device.isNull() && device->isOpen())
        device->close();

    delete outputThread;
    delete loopThread;
}
