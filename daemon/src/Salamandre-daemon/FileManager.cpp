#include <Salamandre-daemon/FileManager.hpp>
#include <Salamandre-daemon/std.hpp>

#include <Salamandre-stats/stats.hpp>

#include <sstream>

namespace salamandre
{
    const std::string FileManager::new_file_dir_path = "datas/save";
    const std::string FileManager::network_file_dir_path = "datas/network";

    bool FileManager::prepareForUpload(int id_medecin)
    {
        int res = 0;
        const std::string path_medecin = std::join("/",std::vector<std::string>({new_file_dir_path,std::to_string(id_medecin)}));
        //check if the medecin dir exist
        if(std::createDir(path_medecin) == 2) //already exist
        {
            //check if the patient dir exist
            const std::vector<std::string> patients = std::getDirList(path_medecin);
            for(const std::string& patient : patients)
            {
                //patient path
                const std::string path_patient = std::join("/",std::vector<std::string>({path_medecin,patient}));
                //get list of files
                const std::vector<std::string> files = std::getFileList(path_patient);

                for(const std::string& file : files)
                {
                    //get list of dest on network
                    auto dests = Stats::get_nodes();
                    //send them
                    for(auto& dest : dests)
                    {
                        const std::string path_save = std::join("/",std::vector<std::string>({network_file_dir_path,
                                                                                             dest->host+":"+std::to_string((int)dest->port),
                                                                                             std::to_string(id_medecin),
                                                                                             patient}));
                        std::cout<<path_save<<" + "<<file<<std::endl;
                    }
                }
                ++res;
            }
        }
        return res;
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
}
