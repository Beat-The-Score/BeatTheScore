#ifndef MIDIOUTPUT_H
#define MIDIOUTPUT_H

#include <vector>
#include <string>

using namespace std;


class MidiOutput
{
public:
    MidiOutput();
    vector<string> getOutputPortList();
};

#endif // MIDIOUTPUT_H
