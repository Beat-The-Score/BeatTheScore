#include "score.h"

Score::Score(QObject *parent)
{
    setParent(parent);
}

int Score::getLength() const
{
    return length;
}

QString Score::getItemId() const
{
    return getFileName();
}

void Score::setTempo(float factor)
{
    for (int i = 0; i < tracks.size(); i++)
    {
        for (int j = 0; j < tracks[i]->notes.size(); j++)
        {
            QPointer<Note> note = tracks[i]->notes[j];
            note->setDuration(note->getDuration() * factor);
            note->setTime(note->getTime() * factor);
        }
    }

    for (int i = 0; i < bars.size(); i++)
    {
        bars[i] *= factor;
    }

    length *= factor;
}

QStringList Score::getTrackNames()
{
    QStringList trackNames;

    for (int i = 0; i < playableTracks.size(); i++)
    {
        trackNames.push_back(QString(playableTracks.at(i)->getName()));
    }

    return trackNames;
}

QString Score::getTitle() const
{
    if (title == "")
    {
        return getTrimmedFileName();
    }
    return title;
}

QString Score::getArtist() const
{
    return artist;
}

QString Score::getFileName() const
{
    return fileName;
}

int Score::getBPM() const
{
    return bpm;
}

QString Score::getTrimmedFileName() const
{
    QString fileName = this->getFileName();
    QString noPathName = fileName.split('/').last();
    QString noEndingName = noPathName.split(".").first();
    QString noSepSignName = noEndingName.replace('_', ' ');

    return noSepSignName;
}
