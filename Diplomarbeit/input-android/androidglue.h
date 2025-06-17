#include <QObject>
#include <QThread>

#ifndef ANDROIDGLUE_H
#define ANDROIDGLUE_H


class AndroidGlue : public QObject
{
    Q_OBJECT

public:
    AndroidGlue();
    int getDescriptor();
    int getVendorID();
    int getProductID();
    int getEndpoint();
    bool isConnected();
    void reset();

};

#endif // ANDROIDGLUE_H
