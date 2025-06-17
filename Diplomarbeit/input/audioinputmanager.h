#ifndef AUDIOINPUTMANAGER_H
#define AUDIOINPUTMANAGER_H

#include <QStringList>
#include <QPointer>

#include "audioinput.h"

class AudioInputManager
{
public:
    AudioInputManager();
    static QStringList getInputPortList();
    static QList<QPointer<Input>>  getInputs(QObject *parent);
};

#endif // AUDIOINPUTMANAGER_H
