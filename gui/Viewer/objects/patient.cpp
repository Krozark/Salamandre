#include "patient.hpp"

Patient::Patient()
{
}

QString Patient::getId()
{
    return this->id;
}

void Patient::setId(QString id)
{
    this->id = id;
}

QString Patient::getDirPath()
{
    return this->dirPath;
}

void Patient::setDirPath(QString dirPath)
{
    this->dirPath = dirPath;
}
