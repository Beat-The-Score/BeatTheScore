#include "visuals.h"

Visuals::Visuals(QQuickItem *parent) {

}

void Visuals::stateChanged(State state)
{
    if (game->getGameMode() == GameMode::TRAINING) {
        switch(state) {
        case State::WAITING:
            game->pauseGameClock();
            return;
        case State::PLAYING:
            game->resumeGameClock();
            return;
        default:
            break;
        }
    }
}

void Visuals::noteOn(Note *note)
{
    qt_noop();
}

void Visuals::setGame(const QObject *game)
{
    if (this->game == game) return;
    this->game = (MainGame*) game;

    //score = this->game->getScore();
    //track = this->game->getTargetTrack();

    //Q_ASSERT(!score.isNull());
    //Q_ASSERT(!track.isNull());
    //Q_ASSERT(!playerTrack.isNull());

    QObject::connect(getGame(), SIGNAL(stateChanged(State)), this,
                     SLOT(stateChanged(State)), Qt::ConnectionType::DirectConnection);

    QObject::connect(getGame()->getCurrentInput().data(), &Input::noteOn, this, &Visuals::noteOn);

    emit gameChanged();
}

MainGame *Visuals::getGame() const
{
    return game;
}


void Visuals::setScrollPosition(qreal position)
{
    scrollMutex.lock();
    scrollBarPosition = (int)((qreal)position * ((qreal)(window()->width() - 40) / (qreal)window()->width()));
    scrollMutex.unlock();
    emit scrollPositionChanged();
}

qreal Visuals::scrollPosition()
{
    scrollMutex.lock();
    qreal ret = scrollBarPosition;
    scrollMutex.unlock();
    return ret;
}

int Visuals::positionToTime() {
    return (int)(((qreal)scrollPosition() / (qreal)window()->width()) * (qreal)game->getLengthOfSelectedSong());
}

void Visuals::setScrollHandleGrabbed(bool grabbed)
{
    this->scrollHandleGrabbed = grabbed;
    if(grabbed) {
        game->setState(State::SCROLLING);
    } else {
        game->setState(State::PLAYING);
    }
}

qint32 Visuals::getRollWidth()
{
    return getTimeFactor() * game->getLengthOfSelectedSong();
}

void Visuals::updateScrollBarPosition()
{
    //qDebug() << "game->getDurationToLastNote() :" << game->getDurationToLastNote();
    if(game->getLengthOfSelectedSong() != 0 && !scrollHandleGrabbed) {
        qreal timeProgress = (qreal)game->getGameTime() / (qreal)game->getLengthOfSelectedSong();
        setScrollPosition(timeProgress * boundingRect().width());
    }
}

void Visuals::updateGameTimeToPosition()
{
    if(game->isReplayMode() && this->scrollHandleGrabbed) {
        if(game->getLengthOfSelectedSong() != 0 && positionToTime() != game->getGameTime()) {
            game->setGameTime(positionToTime());
        }
    }
}

qreal Visuals::getTimeFactor()
{
    //return (qreal) boundingRect().height() / track->getClosestNotes() / maxNotesPerScreen;
    //return (qreal) boundingRect().width() / max(track->getClosestNotes() * maxNotesPerScreen, maxMsPerScreen);
    return (qreal) boundingRect().width() / maxMsPerScreen;
}
