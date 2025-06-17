#ifndef NOTEDISPLAY_H
#define NOTEDISPLAY_H

#include <QQuickItem>
#include <QQmlEngine>
#include <QSet>
#include <mutex>
#include "../music/note.h"
#include "../music/track.h"
#include "visuals.h"
#include "qqi_based_visuals.h"



class ScoreView : public QQI_Based_Visuals
{
    Q_OBJECT

public:
    ScoreView(QQuickItem *parent = 0);
    ~ScoreView();

private:
    qreal toneStep = 0.0;
    int lowerBound = 0;
    int upperBound = 0;
    qreal timeFactor = 0.0;

    // for correctly positioning the note-specific textures (notes, sharp, dot etc.)
    qreal noteXOffset;
    qreal noteYOffset;

    QSGTexture **noteTextures;
    QSGTexture **redNoteTextures;
    QSGTexture **greenNoteTextures;
    QSGTexture *dotTexture = NULL;
    QSGTexture *sharpTexture = NULL;
    QSGTexture *keyTexture = NULL;
    QSGTexture *graceTexture = NULL;
    QSGTexture *slurTexture = NULL;
    std::mutex textureMutex;
    qreal noteWidth, noteHeight;
    QMap<int, qreal> yPositions;

    qreal getDrawPitchForPitch(int pitch);
    bool isSharp(int pitch);
    int numberOfDoubles(int startPitch, int pitch);

    QSGSimpleRectNode* getNodeForNote(QPointer<Note> note, bool isFromScore);

    QSGNode* getScoreViewSpecificNode(qreal x, qreal y, qreal width, qreal height, QSGTexture *texture);

    void loadDisplayOptionSpecificTextures();
    void loadNoteTexture(int i, QRgb color, QSGTexture **destination);
    bool calculateLines();

    QImage colorizeQImage(QImage image, QRgb color);

protected:
    QSGNode* getDisplayOptionSpecificNodes();
};

#endif // NOTEDISPLAY_H
