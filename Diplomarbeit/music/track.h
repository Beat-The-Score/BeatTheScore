#ifndef NOTESTREAM_H
#define NOTESTREAM_H


#include <vector>
#include <memory>

#include <QVariantList>
#include <QQuickView>
#include <QString>

#include "note.h"

using namespace std;


class Track : public QObject
{
    Q_OBJECT
private:
    int minPitch, maxPitch, closestNotes;
    int lastNote;

public:
    void print();

    //TODO: private:
    QString name;
    QList<QPointer<Note>> notes;
    QList<int> firstNoteInSection;
    int gMInstrument;
    int stringCount;
    bool isPercussion = false;
    bool visible = true;

    bool isInputTrack = false; //selected track from the Game Settings menu

    vector<int> tuning;
    QString getName();
    bool operator==(Track &other) const;

    Track(QObject *parent); // no Track without a parent
    //~Track();

    int getMaxPitch() const;
    int getMinPitch() const;
    int getClosestNotes() const;
    int getStartIndex(int sectionIndex);
    QList<QPointer<Note>> getNotesOfSection(int sectionIndex);
    QList<QPointer<Note>> getNotesAt(int time);
    int getNextNotesTime(int time);
    int getStringCount();

    Q_INVOKABLE QString getItemId();

public slots:
    void addNote(QPointer<Note> note);

private slots:
    void deleteNote(QPointer<QObject> obj);

signals:
    void noteAdded(QPointer<Note> note);

};

#endif // NOTESTREAM_H

