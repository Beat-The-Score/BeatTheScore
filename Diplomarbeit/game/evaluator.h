#ifndef EVALUATOR_H
#define EVALUATOR_H

#include <memory>
#include <QMap>

class Evaluator;

#include "../music/note.h"
#include "../music/score.h"
#include "maingame.h"

class Evaluator : public QObject
{
    Q_OBJECT
public:
    Evaluator(QObject* parent, QPointer<MainGame> mainGame, QPointer<Track> target);
    QPointer<Track> target;

    int totalScore;

private:
    void evaluateNote(QPointer<Note> note);
    QMap<QPointer<Note>, pair<QPointer<Note>, int>> reverseMapping;
    QPointer<MainGame> mainGame;
    QList<QPointer<Note>> currentNotes;

private slots:
    void deleteNote(QPointer<QObject> note);

public slots:
    void noteOn(QPointer<Note> note);
    void noteOff(QPointer<Note> note);

signals:
    void noteAdded(QPointer<Note> note);
};

#endif // EVALUATOR_H
