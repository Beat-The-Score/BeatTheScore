#ifndef LICENSEMANAGER_H
#define LICENSEMANAGER_H

#include <QObject>
#include <QtQml>

#ifndef Q_OS_ANDROID
#include <QDir>
#include <QFile>
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/x509.h>
#include <openssl/sha.h>
#include <openssl/pem.h>
#include <iostream>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>

#include "public.pem.h"

#ifdef Q_OS_LINUX
    #include <stdlib.h>
#endif

#endif


enum LicenseCheckingState
{
    UNCHECKED = 0,
    CODE_INVALID = 1,
    LICENSE_RESULT_INVALID = 2,
    LICENSE_FILE_INVALID = 3,
    LICENSE_VALID = 4
};

using namespace std;

class LicenseManager : public QObject
{
    Q_OBJECT

public:
    LicenseManager();

    Q_ENUMS(LicenseCheckingState)
    LicenseCheckingState isLicenseValid(); // Interface for MainGame and QML

public:
    Q_PROPERTY(int licenseValidity READ isLicenseValid NOTIFY isLicenseValidChanged)

    Q_INVOKABLE bool isLicenseFileValid();
    Q_INVOKABLE void isActivationCodeValid(QString email, QString serial);

    Q_ENUMS(LicenseCheckingState)

signals:
    void isLicenseValidChanged();

private slots:
    void licenseReplyFinished(QNetworkReply* reply);

private:
    QString licenseFilePath();
    QString decryptMessage(unsigned char *message, int size);
    QString digestFromString(QString str);

    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    LicenseCheckingState licenseCheckState = UNCHECKED;
};

#endif // LICENSEMANAGER_H
