#ifndef SCORE_H
#define SCORE_H

#include <vector>

#include <QStringList>
#include <QPointer>

#include "track.h"
#include "section.h"

class Score : public QObject
{
    Q_OBJECT
public:
    Score(QObject *parent=NULL);

    int length; // length in milliseconds
    int bpm; // avg. beats per minute
    QList<QPointer<Track>> tracks;
    QList<QPointer<Track>> playableTracks;
    vector<int> bars; // time of each bar
    QList<QPointer<Section>> sections; // bar index of section start
    QStringList getTrackNames();
    QString title, subtitle, artist, album, composer, transcriber, fileName;
    Q_INVOKABLE QString getArtist() const;
    Q_INVOKABLE QString getFileName() const;
    Q_INVOKABLE QString getTitle() const;
    Q_INVOKABLE int getBPM() const;
    Q_INVOKABLE int getLength() const;

    Q_INVOKABLE QString getItemId() const;

    void setTempo(float factor);
    
private:
    QString getTrimmedFileName() const;


};

#endif // SCORE_H
