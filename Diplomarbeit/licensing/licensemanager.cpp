#include "licensemanager.h"

using namespace std;

LicenseManager::LicenseManager()
{
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(licenseReplyFinished(QNetworkReply*)), Qt::DirectConnection);
}

QString LicenseManager::licenseFilePath()
{
    #ifdef Q_OS_LINUX
    #ifndef Q_OS_ANDROID
    QDir configPath(QString(getenv("HOME")) + "/.config/beatthescore");
    if(!configPath.exists()) {
        configPath.mkdir(configPath.path());
    }
    return configPath.absolutePath() + QString("/license");
    #endif
    #endif
    return QDir::currentPath() + "/license";
}

void LicenseManager::licenseReplyFinished(QNetworkReply *reply)
{
    //cout << "licenseReplyFinished();" << endl;
    if(reply->isFinished()) {
        //cout << "isFinished" << endl;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonResult = jsonDoc.object();

        if(jsonResult.contains(QString("result")) &&
                jsonResult["result"].isString() &&
                !jsonResult["result"].toString().isEmpty()) {
            //cout << "result OK" << endl;
            QFile licenseFile(licenseFilePath());
            licenseFile.open(QIODevice::WriteOnly | QIODevice::Text);
            QTextStream fileOut(&licenseFile);
            QString licenseContent = jsonResult["result"].toString();
            fileOut << licenseContent;
            licenseFile.flush();
            licenseFile.close();
            isLicenseFileValid();
        } else {
            //cout << "result NOPE" << endl;
            licenseCheckState = LICENSE_RESULT_INVALID;
            emit isLicenseValidChanged();
        }
    } else {
        licenseCheckState = LICENSE_RESULT_INVALID;
        emit isLicenseValidChanged();
    }
    reply->deleteLater();
}

LicenseCheckingState LicenseManager::isLicenseValid()
{
    return this->licenseCheckState;
}

void LicenseManager::isActivationCodeValid(QString email, QString serial)
{
    licenseCheckState = UNCHECKED;
    //cout << "serial count: " << serial.count() << endl;
    serial = serial.replace(" ", "");
    if(serial.count() != 11) {
        licenseCheckState = CODE_INVALID;
        emit isLicenseValidChanged();
        return;
    }

    bool isCodeValid = true;
    for(int i = 0; i < serial.count() && isCodeValid; i++) {
        switch(i % 4) {
        case 0:
        case 1:
        case 2:
            if(!serial[i].isDigit()) {
                isCodeValid = false;
            }
            break;
        case 3:
            if(serial[i] != '-') {
                isCodeValid = false;
            }
            break;
        }
    }

    if(isCodeValid) {
        manager->get(QNetworkRequest(QUrl("http://beatthescore.at/api/license/get/" +
                                          email +
                                          QString("/") +
                                          serial)));
    } else {
        this->licenseCheckState = CODE_INVALID;
        emit isLicenseValidChanged();
    }
    return;
}


/* What the license file looks like:
 * base64(
 *      RSA_private_encrypt(
 *          [Buyer email address]+[date of purchase] + \n +
 *          hex(
 *              sha256([Buyer email address]+[date of purchase])
 *          )
 *      )
 * )
 * */
bool LicenseManager::isLicenseFileValid()
{
    #ifdef Q_OS_ANDROID
    return true;
    #else

    QFile licenseFile(licenseFilePath());
    //cout << "license Path: " << licenseFilePath().toStdString() << endl;
    if(!licenseFile.exists()) {
        return false;
    }

    licenseFile.open(QIODevice::ReadOnly);
    QByteArray bArr = licenseFile.readAll();
    licenseFile.close();
    QByteArray encryptedMessage = QByteArray::fromBase64(bArr);
    char *encryptedData = encryptedMessage.data();

    QString decryptedMessage = decryptMessage((unsigned char*)encryptedData, encryptedMessage.size());
    // //cout << "Decrypt: " << decryptedMessage.toStdString() << endl;

    //delete encryptedData;

    QStringList decryptedParts = decryptedMessage.split('\n');

    if(decryptedParts.count() == 2 &&
            decryptedParts[1].startsWith(digestFromString(decryptedParts[0]))) {

        licenseCheckState = LICENSE_VALID;
        emit isLicenseValidChanged();
    } else {
        licenseFile.remove();
        licenseCheckState = LICENSE_FILE_INVALID;
        emit isLicenseValidChanged();
    }
    #endif
}

QString LicenseManager::digestFromString(QString str)
{
    #ifndef Q_OS_ANDROID
    EVP_MD_CTX context;
    unsigned char digest[EVP_MAX_MD_SIZE];
    unsigned int digestLength = 0;

    EVP_DigestInit(&context, EVP_sha256());
    EVP_DigestUpdate(&context, str.toStdString().c_str(), str.size());
    EVP_DigestFinal_ex(&context, digest, &digestLength);
    EVP_MD_CTX_cleanup(&context);

    QByteArray bArr((const char*)digest, digestLength);
    QString retStr(bArr.toHex());
    return retStr;
    #endif
}

QString LicenseManager::decryptMessage(unsigned char *message, int size)
{
    #ifndef Q_OS_ANDROID
    int pem_size = sizeof(public_pem)/sizeof(*public_pem);
    BIO *publicKeyBuffer = BIO_new_mem_buf((void*)public_pem, pem_size);
    RSA *rsaPublic;
    EVP_PKEY *pkey = PEM_read_bio_PUBKEY(publicKeyBuffer, NULL, NULL, NULL);
    if (pkey) {
        rsaPublic = EVP_PKEY_get1_RSA(pkey);
        if (rsaPublic) {
            RSA_up_ref(rsaPublic);
        }
        EVP_PKEY_free(pkey);
    }
    if(rsaPublic == NULL) {
        return NULL;
    }
    unsigned char *decryptedMessage = new unsigned char[1024];
    int bufferSize = RSA_public_decrypt(size,
                                        message,
                                        decryptedMessage,
                                        rsaPublic,
                                        RSA_PKCS1_PADDING);
    if(bufferSize == -1) {
        return NULL;
    }

    RSA_free(rsaPublic);
    BIO_free(publicKeyBuffer);
    QString retStr((const char*)decryptedMessage);
    delete [] decryptedMessage;
    if(retStr.isEmpty() || retStr.isNull()) {
        return NULL;
    }
    return retStr;
    #endif
}
