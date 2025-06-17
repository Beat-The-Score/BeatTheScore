#ifndef MIDIINPUTMANAGER_H
#define MIDIINPUTMANAGER_H

#include <string>
#include <vector>

#include <QList>
#include <QPointer>

#include "midiinput.h"

using namespace std;


class MidiInputManager
{
public:
    MidiInputManager();
    //static QStringList getInputPortList();
    QList<QPointer<Input>>  getInputs(QObject *parent);

private:
    QList<QPointer<Input>> midiInputs;
};

#endif // MIDIINPUTMANAGER_H
