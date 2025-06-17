#include <QVariant>
#include <QMetaObject>
#include <QQuickView>

#include "soundnavigationhandler.h"
#include "../music/note.h"

SoundNavigationHandler::SoundNavigationHandler(QQuickView *parent)
{
    this->setParent(parent);
    this->guiItem = parent;
}

void SoundNavigationHandler::passCommand(QList<int> notes)
{
    QQuickItem *rootObject = guiItem->rootObject();
    if (!rootObject) return;

    QVariant returnedValue;
    QVariant msg = QVariant::fromValue(notes);
    QMetaObject::invokeMethod((QObject*) rootObject, "handleCommand", Q_ARG(QVariant, msg));
}

void SoundNavigationHandler::noteOn(QPointer<Note> note)
{
    notes.append(note);
    if (notes.size() == 3) {
        QList<int> pitches;
        for (int i = 0; i< notes.size(); i++)
        {
            if (!notes.at(i).isNull())
                pitches.append(notes.at(i)->getPitch());
        }
        passCommand(pitches);
        notes.clear();
    }
}

void SoundNavigationHandler::noteOff(QPointer<Note> note)
{

    int index = notes.indexOf(note);
    notes.removeAt(index);
}

void SoundNavigationHandler::changeFullScreen()
{
    if (guiItem->visibility() == guiItem->FullScreen)
    {
        guiItem->showNormal();
    }
    else {
        guiItem->showFullScreen();
    }
}
