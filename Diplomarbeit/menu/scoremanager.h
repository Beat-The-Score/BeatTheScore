#ifndef SCOREMANAGER_H
#define SCOREMANAGER_H

#include <QList>
#include <QPointer>

#include "../music/score.h"

class ScoreManager
{
public:
    ScoreManager();
    QList<QPointer<Score>> getScoresInFolder(QString folderPath);
    
};

#endif // SCOREMANAGER_H
