#ifndef GP5READER_H
#define GP5READER_H

#include <QPointer>

#include "track.h"
#include "score.h"

class GP5Reader
{
public:
    GP5Reader();

    string read(QPointer<Score> score, QString filename);
    string readHead(QPointer<Score> score, string filename);
};

#endif // GP5READER_H
