#ifndef SoundNavigationHandler_H
#define SoundNavigationHandler_H

#include <QObject>
#include <QQuickView>
#include <QPointer>
#include <QList>

#include "../music/note.h"

class SoundNavigationHandler : public QObject
{
    Q_OBJECT
public:
    SoundNavigationHandler(QQuickView *parent = 0);
    void passCommand(QList<int> notes);

public slots:
    void noteOn(QPointer<Note> note);
    void noteOff(QPointer<Note> note);

private:
    QPointer<QQuickView> guiItem;
    QList<QPointer<Note>> notes;

public:
    void changeFullScreen();
};

#endif // SoundNavigationHandler_H
