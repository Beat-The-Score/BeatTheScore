#ifndef TABLATUREVIEW_H
#define TABLATUREVIEW_H

#include "qqi_based_visuals.h"
#include "visuals.h"


class TablatureView : public QQI_Based_Visuals
{
    Q_OBJECT

private:
    qreal pitchesPerScreen;
    qreal pitchOffset;
    qreal pitchHeight;

    int trackIndex;
    int startMs;
    int numOfStrings;

    qreal verticalBorderPercentage = 0.15;
    qreal top;
    qreal availableHeight;
    qreal stringSpacing;

    qreal getXForTime(int time);
    qreal getYForPitch(int pitch);


    int interval = 20;

    qreal getYForString(int number);
    QSGNode *getStringsNode();
    int getStringCount();

    virtual QSGSimpleRectNode* getNodeForNote(QPointer<Note> note, bool isFromScore = false);
    QSGGeometryNode* getBendingNode(QPointer<Note> note, qreal left, qreal width, qreal y_Bending, qreal bendingHeight);

    int getBendindSegmentCount();
    QList<int> getPitches(QPointer<Note> note);

    QSGTexture *hammerOnTexture = NULL;
public:
    TablatureView(QQuickItem* parent = 0);

protected:
    //QSGNode *updatePaintNode(QSGNode *node, UpdatePaintNodeData *data);
    QSGNode *getDisplayOptionSpecificNodes();
    void loadDisplayOptionSpecificTextures();
};

#endif // TABLATUREVIEW_H
