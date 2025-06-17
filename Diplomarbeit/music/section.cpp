#include "section.h"

Section::Section(int index, int time, int length, QString name) : index(index), time(time), length(length), name(name)
{
}

QString Section::getName() const
{
    return name;
}

int Section::getLength() const
{
    return length;
}

QString Section::getItemId() const
{
    return getName();
}

int Section::getTime() const
{
    return time;
}
