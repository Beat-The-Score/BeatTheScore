#ifndef SECTION_H
#define SECTION_H

#include <QString>
#include <QObject>

class Section : public QObject
{
    Q_OBJECT


public:
    Section(int index, int time, int length, QString name);

    int index;
    int time;
    int length;
    QString name;
    Q_INVOKABLE int getTime() const;
    Q_INVOKABLE QString getName() const;
    Q_INVOKABLE int getLength() const;

    Q_INVOKABLE QString getItemId() const;
};


#endif // SECTION_H
