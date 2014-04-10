#include "doctor.hpp"

Doctor::Doctor()
{
}

Doctor::~Doctor()
{
}

void Doctor::setId(QString id){
    this->id = id;
}

void Doctor::setPass(QString pass){
    this->pass = pass;
}

void Doctor::setDirPath(QString dirPath){
    this->dirPath = dirPath;
}

void Doctor::setType(TypeDoctor type){
    this->type = type;
}

QString Doctor::getId(){
    return this->id;
}

QString Doctor::getPass(){
    return this->pass;
}

QString Doctor::getDirPath(){
    return this->dirPath;
}

Doctor::TypeDoctor Doctor::getType(){
    return this->type;
}


