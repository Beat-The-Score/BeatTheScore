#include <iostream>
#include <functional>
#include <chrono>

#include "evaluator.h"
#include "../music/note.h"
#include "maingame.h"


Evaluator::Evaluator(QObject* parent, QPointer<MainGame> mainGame, QPointer<Track> target)
{
    setParent(parent);

    this->mainGame = mainGame;
    this->target = target;

    totalScore = 0;
}

void Evaluator::evaluateNote(QPointer<Note> note) {
    totalScore -= note->getScore(); // to avoid counting the note twice

    int startIndex = target->getStartIndex(mainGame->getSelectedSectionIndex());
    int sectionOffset = 0;
    int firstSectionTime = 0;
    if (mainGame->getScore()->sections.size() > 0)
    {
        firstSectionTime = mainGame->getScore()->sections.at(0)->getTime();
        sectionOffset = mainGame->getScore()->sections.at(mainGame->getSelectedSectionIndex())->getTime() - firstSectionTime;
    }

    int bestDistance = -1;
    QPointer<Note> match;
    for (int i=startIndex; i<target->notes.size(); i++)
    {
        QPointer<Note> currentNote = target->notes[i];
        if (!currentNote->getMuted())
        {
            if (note->getPitch() == 0 || currentNote->getPitch() == note->getPitch())
            {
                int distance = abs(note->getTime() - currentNote->getTime() + sectionOffset);
                if (note->getDuration() > 0)
                {
                    //distance = (distance + abs(note->getDuration() - currentNote->getTieDuration())) / 2;
                }
                if (distance < 1000 && (bestDistance == -1 || bestDistance > distance))
                {
                    bestDistance = distance;
                    match = currentNote;
                    match->setTime(match->getTime() - sectionOffset);
                }
            }
        }
    }

    if (!match.isNull())
    {
        if (!reverseMapping.contains(match)) // no note has match as target yet
        {
            reverseMapping[match] = pair<QPointer<Note>, int>(note, bestDistance);
            note->setTarget(match);
            note->setScore(min(max(1000 + 20 - bestDistance, 0), 1000) / 10);
        }
        else if (note->getTarget() != match) // another note has match as target
        {
            if (reverseMapping[match].second > bestDistance) // new note is closer
            {
                if (reverseMapping[match].first.isNull())
                {
                    cout << "NULL-Note! Something went wrong!" << endl;
                }
                else
                {
                    reverseMapping[match].first->setTarget(NULL);
                }
                reverseMapping[match] = pair<QPointer<Note>, int>(note, bestDistance);
                note->setTarget(match);
            }
            else
            {
                note->setTarget(NULL);
            }
        }
        else // this note has match as target, just update the entry
        {
            reverseMapping[match] = pair<QPointer<Note>, int>(note, bestDistance);
        }
    }
    else
    {
        note->setTarget(NULL);
        note->setScore(0);
    }
    //cout << note->getScore() << endl;
    totalScore += note->getScore();
}

void Evaluator::noteOn(QPointer<Note> note)
{
    if (mainGame->isRunning())
    {
        currentNotes.append(note);
        if (mainGame->getGameMode() == GameMode::NORMAL)
        {
            if (mainGame->isRunning())
            {
                int timeLatency =
                        note->getRealTime() -
                        chrono::time_point_cast<chrono::milliseconds>(
                            chrono::steady_clock::now()).time_since_epoch().count();
                //cout << "note latency = " << timeLatency << endl;
                note->setTime(mainGame->getGameTime() + timeLatency);
                //connect(note.data(), &QObject::destroyed, this, &Evaluator::deleteNote);
                evaluateNote(note);
                emit noteAdded(note);
            }
        }
        else // trainings mode
        {
            // could be done in advance
            int time = target->getNextNotesTime(mainGame->getGameTime());
            QList<QPointer<Note>> targetNotes = target->getNotesAt(time);

            if (targetNotes.size() == currentNotes.size())
            {
                bool correct = true;
                for (int i=0; i<currentNotes.size(); i++)
                {
                    bool found = false;
                    for (int j=0; j<targetNotes.size(); j++)
                    {
                        if (targetNotes[j]->getPitch() == currentNotes[i]->getPitch())
                        {
                            found = true;
                            break;
                        }
                    }

                    if (!found)
                    {
                        correct = false;
                        break;
                    }
                }

                if (correct)
                {
                    note->setTime(time);
                    mainGame->nextNote(time);
                    emit noteAdded(note);
                }
            }
        }
    }
}

void Evaluator::noteOff(QPointer<Note> note)
{
    for (int i=0; i<currentNotes.size(); i++)
    {
        if (!currentNotes[i].isNull() && currentNotes[i]->getPitch() == note->getPitch()) {
            currentNotes[i]->setDuration(max(mainGame->getGameTime() - note->getTime(), 1));
            evaluateNote(currentNotes[i]);
        }
    }
    currentNotes.removeAll(note);
}

void Evaluator::deleteNote(QPointer<QObject> note)
{
    reverseMapping.remove(dynamic_cast<Note*>(note.data()));
}
