#ifndef VISUALS_H
#define VISUALS_H

#include <QObject>
#include <QQuickItem>
#include <QPointer>
#include <QTimer>
#include <mutex>

#include "../game/maingame.h"
#include "../game/maingamestate.h"

class Visuals : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(MainGame* game READ getGame WRITE setGame NOTIFY gameChanged)
    Q_PROPERTY(qreal scrollPosition READ scrollPosition WRITE setScrollPosition NOTIFY scrollPositionChanged)

public:
    Q_INVOKABLE void updateScrollBarPosition();
    Q_INVOKABLE void updateGameTimeToPosition();
    Q_INVOKABLE void setScrollPosition(qreal position);
    Q_INVOKABLE void setScrollHandleGrabbed(bool grabbed);
    Q_INVOKABLE qint32 getRollWidth(); // just roll width it
    Q_INVOKABLE qreal scrollPosition();
    Q_INVOKABLE qreal getTimeFactor();

public:
    Visuals(QQuickItem *parent = 0);
    virtual ~Visuals() {}

    // Get Methods
    MainGame *getGame() const;

    // Set Methods
    virtual void setGame(const QObject *getGame);

    //QPointer<Score> score;
    //QPointer<Track> track;

protected:
    qreal scrollBarPosition = 0;
    bool scrollHandleGrabbed = false;
    qreal maxMsPerScreen = 5000;
    qreal maxNotesPerScreen = 100;

    std::mutex scrollMutex;
    MainGame* game;

    int positionToTime();

public slots:
    void stateChanged(State state);
    virtual void noteOn(Note* note);

signals:
    void gameChanged();
    void scrollPositionChanged();
};

#endif // VISUALS_H
