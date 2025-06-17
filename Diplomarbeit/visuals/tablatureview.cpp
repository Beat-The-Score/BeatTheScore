#include <iostream>
#include <memory>
#include <algorithm>

#include <QQuickItem>
#include <QSGGeometry>
#include <QSGFlatColorMaterial>
#include <QSGGeometryNode>
#include <QSGSimpleRectNode>
#include <QSGSimpleTextureNode>
#include <QTimer>
#include "tablatureview.h"
#include "../music/note.h"
#include "../music/section.h"
#include "visuals.h"
#include "qqi_based_visuals.h"
#include "../music/track.h"

using namespace std;


TablatureView::TablatureView(QQuickItem *parent) : QQI_Based_Visuals(parent)
{
    // Important, otherwise the paint method is never called
    setFlag(ItemHasContents);
    /*
    rightTriangleTexture = NULL;
    leftTriangleTexture = NULL;
    pianoTexture = NULL;
    */
    startIndex = - 1;
}


qreal TablatureView::getYForString(int index) {

    numOfStrings = game->getScore()->tracks.at(game->getSelectedTrackIndex())->stringCount;

    top = boundingRect().top() + boundingRect().height() * verticalBorderPercentage;
    availableHeight = boundingRect().height() * (1 - verticalBorderPercentage);
    stringSpacing = availableHeight / numOfStrings;

    return top + stringSpacing * index;
}


qreal TablatureView::getYForPitch(int pitch) {
    //qreal pitchPosition[] = {0, 2, 3, 5, 6, 8, 10, 11, 13, 14, 16, 17}; // 19
    //return boundingRect().bottom() - ((pitchPosition[pitch % 12] / 19 + pitch / 12) * 12 - pitchOffset) * pitchHeight;
    return boundingRect().bottom() - (pitch - pitchOffset) * pitchHeight;
}


qreal TablatureView::getXForTime(int time) {
    return boundingRect().right() - (qreal) boundingRect().width() / 2 + (time - gameTime) * getTimeFactor();
}



QSGNode* TablatureView::getStringsNode() {

    QSGNode* stringNode = new QSGNode();

    for (int i = 0; i < getStringCount(); i++) {
        // Draw vertical line to indicate "current position"

        qreal stringLevel = getYForString(i);
        QSGNode* lineNode = getLineNode(boundingRect().left(), stringLevel, boundingRect().right(), stringLevel);
        stringNode->appendChildNode(lineNode);
    }

    return stringNode;
}




int TablatureView::getStringCount() {
    return game->getScore()->tracks.at(game->getSelectedTrackIndex())->getStringCount();
}


int TablatureView::getBendindSegmentCount()
{
    return 20;
}


QSGGeometryNode* TablatureView::getBendingNode(QPointer<Note> note, qreal left, qreal width, qreal y_Bending, qreal bendingHeight)
{
    QList<int> pitches = getPitches(note);
    QSGGeometryNode *node = 0;
    QSGGeometry *geometry = 0;

    node = new QSGGeometryNode;
    geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), pitches.size());
    geometry->setLineWidth(2);
    geometry->setDrawingMode(GL_LINE_STRIP);
    node->setGeometry(geometry);
    node->setFlag(QSGNode::OwnsGeometry);
    QSGFlatColorMaterial *material = new QSGFlatColorMaterial;
    material->setColor(Qt::black);
    node->setMaterial(material);
    node->setFlag(QSGNode::OwnsMaterial);

    QSGGeometry::Point2D *vertices = geometry->vertexDataAsPoint2D();

    qreal x_interval = (width / note->getDuration()) * interval;
    qreal y_interval = bendingHeight / 200; // 200 cents = 2 semitones

    for (int i = 0; i < pitches.size(); ++i) {

        float x = left + i * x_interval;
        float y = y_Bending - pitches[i] * y_interval;

        vertices[i].set(x, y);
    }

    return node;
}


QList<int> TablatureView::getPitches(QPointer<Note> note)
{
    QList<int> pitches;

    if (note->pitchShift.length() == 0) {
        return pitches;
    }

    int time = 0;

    while (time < note->getDuration()) {
        pitches.append(note->getPitchShift(time));
        time += interval;
    }

    return pitches;
}


QSGSimpleRectNode* TablatureView::getNodeForNote(QPointer<Note> note, bool isFromScore)
{
    if (note->getMuted())
    {
        return NULL;
    }

    QSGSimpleRectNode* newRectNode = new QSGSimpleRectNode();
    QRectF nodeRectangle;

    int string;
    if (isFromScore) {
        string  = note->getString();
    }
    else {
        if (note->getTarget().isNull())
            string = -2;
        else
            string = note->getTarget()->getString();
    }

    qreal stringLevel = getYForString(string);
    qreal left = calculatePosition(note, isFromScore);

    qreal height = (availableHeight / numOfStrings) * 0.8;
    qreal width = getNoteRectangleWidth(note);

    stringLevel += height * 0.2;

    nodeRectangle = QRectF(left, stringLevel, width, height);
    newRectNode->setRect(nodeRectangle);
    newRectNode->setColor(Qt::transparent);

    qreal factor_NoteRectangle = 0.25;
    qreal factor_FretNumber = 0.3;
    qreal factor_Bending = 0.3;
    qreal factor_Rating = 0.15;

    // Note Rectangle
    qreal height_NoteRectangle = height * factor_NoteRectangle;
    qreal y_NoteRectangle = stringLevel + (1 - factor_NoteRectangle) * height;

    // Fret Number
    qreal height_FretNumber = height * factor_FretNumber;
    qreal y_fretNumber = stringLevel + (1 - (factor_NoteRectangle + factor_FretNumber)) * height;

    // Bending Profile
    qreal height_Bending = height * factor_Bending;
    qreal y_Bending = stringLevel + (1 - (factor_NoteRectangle + factor_FretNumber + factor_Bending)) * height;

    // Rating
    qreal height_Rating = height * factor_Rating;
    qreal y_Rating = stringLevel + (1 - (factor_NoteRectangle + factor_FretNumber + factor_Bending + factor_Rating)) * height;

    if (isFromScore)
    {
        // Note Rectanlge
        QSGSimpleRectNode* noteRectangleNode = getBorderedRectangleNode(left, y_NoteRectangle, width, height_NoteRectangle);
        newRectNode->appendChildNode(noteRectangleNode);

        // Fret Number
        QSGSimpleRectNode* numberNode = getNumberNode(note->getFret(), height_FretNumber, left, y_fretNumber);
        newRectNode->appendChildNode(numberNode);

        // Bending
        newRectNode->appendChildNode(getBendingNode(note, left, width, y_Bending, height_Bending));

        qreal slideLeft = left + numberNode->rect().width() + width * 0.2;
        qreal slideRight = left + width * 0.8;

        // Slide
        if (note->getLinking() == LinkingType::slide) {
            newRectNode->appendChildNode(getLineNode(slideLeft, y_fretNumber, slideRight, y_fretNumber, Qt::black, 1));
        }

        // Hammer-On / Pull-Off
        else if (note->getLinking() == LinkingType::abrupt) {

            QSGSimpleTextureNode* hoNode = new QSGSimpleTextureNode();
            QRectF rect(slideLeft, y_fretNumber, width * 0.4, height_FretNumber * 0.3);
            hoNode->setRect(rect);
            hoNode->setTexture(hammerOnTexture);
            newRectNode->appendChildNode(hoNode);
        }
    }

    else
    {
        if (!note->getTarget().isNull()) {

            if (note->pitchOk()) {
                width = getNoteRectangleWidth(note);
                QSGSimpleRectNode* playedNoteRectangleNode = getPlayerNoteRectanlgeNode(note, left, y_NoteRectangle, width, height_NoteRectangle);
                newRectNode->appendChildNode(playedNoteRectangleNode);

                // Rating
                if (note->getDuration() > 0) {
                    QSGSimpleRectNode *number = getNumberNode(note->getScore(), height_Rating, left, y_Rating);
                    newRectNode->appendChildNode(number);
                }
            }
            else {
                qreal y_target = getYForString(note->getTarget()->getString());
                qreal x_target = calculatePosition(note->getTarget(), true);
                QSGSimpleTextureNode* arrowNode = new QSGSimpleTextureNode();
                QRectF rect(x_target, y_target, width * 0.4, height_FretNumber * 0.3);
                arrowNode->setRect(rect);
                arrowNode->setTexture(leftTriangleTexture);
                newRectNode->appendChildNode(arrowNode);

                /*
                qreal crossLeft = calculatePosition(note->getTarget(), isFromScore);
                qreal crossTop = getYForString(note->getTarget()->getString());
                QRectF crossRect(crossLeft, crossTop, 20, 20);
                newRectNode->appendChildNode(new QSGSimpleRectNode(crossRect, Qt::red));
                */
            }
        }

    }

    return newRectNode;
}


QSGNode *TablatureView::getDisplayOptionSpecificNodes()
{
    return getStringsNode();
}


void TablatureView::loadDisplayOptionSpecificTextures()
{
    hammerOnTexture = loadTextureFromSVG(QString("graphics/notes/tie.svg"), width(), height());
}




