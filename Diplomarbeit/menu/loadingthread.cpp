#include "loadingthread.h"

LoadingThread::LoadingThread(QObject *parent) : QThread(parent)
{
    connect(this, SIGNAL(started()), this, SLOT(startLoading()), Qt::DirectConnection);
}

void LoadingThread::startLoading()
{
    game->start();
    this->terminate();
    disconnect(this, SIGNAL(started()), this, SLOT(startLoading()));
}
