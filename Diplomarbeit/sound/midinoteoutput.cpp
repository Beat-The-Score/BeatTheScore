#include "midinoteoutput.h"

MidiNoteOutput::MidiNoteOutput(shared_ptr<AudioBuffer> audioBuffer, QPointer<MainGame> game)
{
    this->audioBuffer = audioBuffer;
    this->mainGame = game;
    setParent(game);
}

void MidiNoteOutput::passSamples(NoteSamples* samples)
{
    this->samples[samples->getInstrumentType()] = samples;
}

void MidiNoteOutput::setMusicInstrument(MusicInstrument instrument)
{
    this->instrumentType = instrument;
}

void MidiNoteOutput::noteOn(Note *note)
{
    unsigned int id = audioBuffer->writeData(samples[instrumentType]->getNote(note->getPitch()), true);
    pitchIdMap.insert(note->getPitch(), id);
}

void MidiNoteOutput::noteOff(Note *note)
{
    unsigned int id = 0;
    if(!pitchIdMap.isEmpty() && pitchIdMap.contains(note->getPitch())) {
        id = pitchIdMap[note->getPitch()];
        audioBuffer->stopId(id);
    }
}
