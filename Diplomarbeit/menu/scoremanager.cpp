#include <QList>
#include <QPointer>
#include <QDir>

#include "scoremanager.h"
#include "../music/score.h"
#include "../music/gp5reader.h"

ScoreManager::ScoreManager()
{
}

QList<QPointer<Score>> ScoreManager::getScoresInFolder(QString folderPath)
{
    GP5Reader fileReader;
    QList<QPointer<Score>> scores;
    QDir dir(folderPath);
    QString path = dir.absolutePath();
    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
            QFileInfo fileInfo = list.at(i);
            if (fileInfo.fileName().contains("gp")) {
                QPointer<Score> newScore = new Score();
                fileReader.readHead(newScore, fileInfo.filePath().toStdString());
                scores.append(newScore);
            }
    }

    return scores;
}
