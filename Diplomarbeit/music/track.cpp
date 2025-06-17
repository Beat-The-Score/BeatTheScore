#include <vector>
#include <memory>
#include <iostream>

#include <QMetaType>
/*
#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeContext>
*/
#include <QQuickView>
#include <QQmlContext>
#include <qqml.h>
#include <QPointer>

#include "track.h"
#include "note.h"

using namespace std;



int Track::getMaxPitch() const
{
    return maxPitch;
}

int Track::getMinPitch() const
{
    //return 20; // temp
    return minPitch;
}

int Track::getClosestNotes() const
{
    return closestNotes;
}

void Track::print()
{
    for (unsigned int i = 0; i < notes.size(); i++)
    {
        QPointer<Note> event = notes.at(i);
        cout << event->getTime() << "  Pitch: " << event->getPitch() << endl;
    }
    cout << notes.size() << " notes" << endl;
}

QString Track::getName()
{
    if (this->name != "")
    {
        return this->name;
    }
    return "unnamed";
}

bool Track::operator==(Track &other) const
{
    return other.notes == this->notes;
}

void Track::addNote(QPointer<Note> note)
{
    //connect(note.data(), &QObject::destroyed, this, &Track::deleteNote);
    //note->setParent(this);
    minPitch = min(note->getPitch(), minPitch);
    maxPitch = max(note->getPitch(), maxPitch);

    int distance = note->getTime() - lastNote;
    if (!note->getGrace()) {
        if (closestNotes == 0 || (distance > 0 && distance < closestNotes)) {
            closestNotes = distance;
        }
        lastNote = note->getTime();
    }
    notes.append(note);
    emit noteAdded(note);
}

void Track::deleteNote(QPointer<QObject> obj)
{
    //notes.erase(remove(notes.begin(), notes.end(), obj), notes.end());
}

Track::Track(QObject *parent)
{
    setParent(parent);
    minPitch = 127;
    maxPitch = 0;
    closestNotes = 0;
    lastNote = 0;
}

int Track::getStartIndex(int sectionIndex)
{
    if (sectionIndex < firstNoteInSection.size())
    {
        return this->firstNoteInSection.at(sectionIndex);
    }
    return 0;
}

QList<QPointer<Note> > Track::getNotesOfSection(int sectionIndex)
{
    return notes.mid(getStartIndex(sectionIndex));
}

QList<QPointer<Note> > Track::getNotesAt(int time)
{
    QList<QPointer<Note> > currentNotes;
    for (int i=0; i<notes.size(); i++)
    {
        if (notes[i]->getTime() == time)
        {
            currentNotes.append(notes[i]);
        }
    }
    return currentNotes;
}

int Track::getNextNotesTime(int time)
{
    for (int i=0; i<notes.size(); i++)
    {
        if (notes[i]->getTime() >= time)
        {
            return notes[i]->getTime();
        }
    }
}

int Track::getStringCount()
{
    return stringCount;
}

QString Track::getItemId()
{
    return getName();
}
