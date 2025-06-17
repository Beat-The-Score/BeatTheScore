#ifndef PIANOROLL_H
#define PIANOROLL_H

#include <QQuickItem>
#include <QSGGeometry>
#include <QSGFlatColorMaterial>
#include <QSGSimpleRectNode>
#include <QMutex>
#include <QMutexLocker>

#include "../music/track.h"
#include "../game/maingame.h"
#include "visuals.h"
#include "qqi_based_visuals.h"

class PianoRoll : public QQI_Based_Visuals
{
    Q_OBJECT
    //Q_PROPERTY(MainGame* game READ getGame WRITE setGame NOTIFY gameChanged)

public:
    PianoRoll(QQuickItem* parent = 0);
    ~PianoRoll();


private:
    vector<shared_ptr<Note>> newNotes;
    QMap<int, int> noteLight;

    qreal pitchesPerScreen;
    qreal pitchOffset;
    qreal pitchHeight;

    QSGTexture* pianoTexture;

    qreal getYForPitch(int pitch);
    qreal getXForTime(int time);

    virtual QSGSimpleRectNode* getNodeForNote(QPointer<Note> note, bool isFromScore = false);
public slots:
    void loadDisplayOptionSpecificTextures();
    void noteOn(Note* note);

protected:
    QSGNode *getDisplayOptionSpecificNodes();
};

QML_DECLARE_TYPE(PianoRoll)

#endif // PIANOROLL_H
