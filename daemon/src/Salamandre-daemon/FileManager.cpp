#include <Salamandre-daemon/FileManager.hpp>

#include <sstream>

namespace salamandre
{
    std::string FileManager::new_file_dir_path = "datas/save/";
    std::string FileManager::network_file_dir_path = "datas/network/";

    bool FileManager::prepareForUpload(int id_medecin)
    {
        //check if the medecin dir exist
        //check if the patient dir exist
        //get list of files

        //get list of dest on network
        //send them
        return true;
    }

    bool FileManager::prepareForUpload(int id_medecin,int id_patient)
    {
        return true;
    }

    bool FileManager::prepareForUpload(int id_medecin,int id_patient,std::string filename)
    {
        return true;
    }

    std::string FileManager::getFilePath(int id_medecin,int id_patient, std::string filename)
    {
        std::stringstream stream;
        stream<<id_medecin<<"/"<<id_patient<<"/"<<filename;
        return stream.str();
    }

    //////// Private /////////////
    int FileManager::createDir(std::string dirpath)
    {
        int res = 0; //0 => error, 1 => created, 2 => already exist
        return res;
    }
}
