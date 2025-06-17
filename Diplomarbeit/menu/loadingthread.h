#ifndef LOADINGTHREAD_H
#define LOADINGTHREAD_H

#include <QObject>
#include <QPointer>
#include "../game/maingame.h"

class LoadingThread : public QThread
{
    Q_OBJECT
public:
    LoadingThread(QObject *parent = 0);
    QPointer<MainGame> game;

signals:

public slots:
    void startLoading();
};

#endif // LOADINGTHREAD_H
