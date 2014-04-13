#include <Salamandre-daemon/FileManager.hpp>
#include <Salamandre-daemon/std.hpp>

#include <Salamandre-stats/stats.hpp>

#include <sstream>
#include <stdlib.h>

namespace salamandre
{
    const std::string FileManager::new_file_dir_path = "daemon/tosave";
    const std::string FileManager::network_file_dir_path = "daemon/network";

    bool FileManager::prepareForUpload(int id_medecin)
    {
        int res = 0;
        const std::string path_medecin = std::join("/",std::vector<std::string>({new_file_dir_path,
                                                                                std::to_string(id_medecin)}));
        //check if the medecin dir exist
        if(std::createDir(path_medecin) == 2) //already exist
        {
            //check if the patient dir exist
            const std::vector<std::string> patients = std::getDirList(path_medecin);
            for(const std::string& patient : patients)
                res += prepareForUpload(id_medecin,::atoi(patient.c_str()));
        }
        return res;
    }

    bool FileManager::prepareForUpload(int id_medecin,int id_patient)
    {
        int res = 0;
        //patient path
        const std::string path_patient = std::join("/",std::vector<std::string>({new_file_dir_path,
                                                                                std::to_string(id_medecin),
                                                                                std::to_string(id_patient)}));
        if(std::createDir(path_patient) == 2) //already exist
        {
            //get list of files
            const std::vector<std::string> files = std::getFileList(path_patient);
            for(const std::string& file : files)
                res += prepareForUpload(id_medecin,id_patient,file);
        }
        return res;
    }

    bool FileManager::prepareForUpload(int id_medecin,int id_patient,std::string filename)
    {
        int res = 0;
        const std::string path_file = std::join("/",std::vector<std::string>({new_file_dir_path,
                                                                                std::to_string(id_medecin),
                                                                                std::to_string(id_patient),
                                                                                filename}));
        //test if file exist
        //get list of dest on network
        auto dests = Stats::get_nodes();
        //send them
        for(auto& dest : dests)
        {
            const std::string path_save = std::join("/",std::vector<std::string>({network_file_dir_path,
                                                                                 dest->host+":"+std::to_string((int)dest->port),
                                                                                 std::to_string(id_medecin),
                                                                                 std::to_string(id_patient),
                                                                                 filename}));
            //TODO cp filename to path_save
        }
        //TODO rm filename
        std::cout<<"save file "<<path_file<<std::endl;
        return res;
    }

    std::string FileManager::getFilePath(int id_medecin,int id_patient, std::string filename)
    {
        std::stringstream stream;
        stream<<id_medecin<<"/"<<id_patient<<"/"<<filename;
        return stream.str();
    }
}
