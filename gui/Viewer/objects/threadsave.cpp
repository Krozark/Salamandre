#include <objects/threadsave.hpp>
#include <objects/socksender.hpp>

threadSave::threadSave(salamandre::Patient *patient, salamandre::Doctor *doctor, QObject *parent) :
    QThread(parent), doctor(doctor), patient(patient)
{

}

void threadSave::run()
{
    int val = 0;

    if(this->saveFEC){
        emit setProgressText("<span style=\"color:orange\">Starting to save FEC</span>");

        salamandre::RegistryRecord *record = this->patient->getRegistryRecord();
        record->save(this->doctor->getPass().toStdString());
        sockSender::sendFile(this->doctor->getId().toInt(), this->patient->getId().toInt(), record->getFileName());

        emit setProgressText("<span style=\"color:darkgreen\">FEC has been saved</span>");

        ++val;
        this->saveFEC = false;
    }

    emit saveProgress(SAVE_OF_FEC);
    emit setProgress(val*100/RECORD_TO_SAVE);

    if(this->saveFCT){
        emit setProgressText("<span style=\"color:orange\">Starting to save FCT</span>");

        salamandre::ConfidentialRecord *record = this->patient->getConfidentialRecord();
        record->save(this->doctor->getPass().toStdString());
        sockSender::sendFile(this->doctor->getId().toInt(), this->patient->getId().toInt(), record->getFileName());

        emit setProgressText("<span style=\"color:darkgreen\">FCT has been saved</span>");

        ++val;
        this->saveFCT = false;
    }

    emit saveProgress(SAVE_OF_FCT);
    emit setProgress(val*100/RECORD_TO_SAVE);

    if(this->saveFMT){
        emit setProgressText("<span style=\"color:orange\">Starting to save FMT</span>");

        salamandre::MedicalRecord *record = this->patient->getMedicalRecord();
        record->save(this->doctor->getPass().toStdString());
        sockSender::sendFile(this->doctor->getId().toInt(), this->patient->getId().toInt(), record->getFileName());

        emit setProgressText("<span style=\"color:darkgreen\">FMT has been saved</span>");

        ++val;
        this->saveFMT = false;
    }

    emit saveProgress(SAVE_OF_FMT);
    emit setProgress(val*100/RECORD_TO_SAVE);

    if(this->saveFMN){
        emit setProgressText("<span style=\"color:orange\">Starting to save FMN</span>");

        salamandre::DigitalRecord *record = this->patient->getDigitalRecord();
        record->save();
        sockSender::sendFile(this->doctor->getId().toInt(), this->patient->getId().toInt(), record->getFileName());

        emit setProgressText("<span style=\"color:darkgreen\">FMN has been saved</span>");

        ++val;
        this->saveFMN = false;
    }

    emit saveProgress(SAVE_OF_FMN);
    emit setProgressText("All records has been saved");
    emit setProgress(val*100/RECORD_TO_SAVE);
}

