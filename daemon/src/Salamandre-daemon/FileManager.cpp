#include <Salamandre-daemon/FileManager.hpp>
#include <Salamandre-daemon/std.hpp>

#include <Salamandre-stats/stats.hpp>

#include <sstream>
#include <cstdlib>
#include <sys/file.h>

namespace salamandre
{
    const std::string FileManager::new_file_dir_path = "datas/tosave";
    const std::string FileManager::network_file_dir_path = "datas/network";

    bool FileManager::prepareForUpload(int id_medecin)
    {
        int res = 0;
        const std::string path_medecin = std::join("/",std::vector<std::string>({new_file_dir_path,
                                                                                std::to_string(id_medecin)}));
        //check if the medecin dir exist
        //if(std::createDir(path_medecin) == 2) //already exist
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
        //if(std::createDir(path_patient) == 2) //already exist
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
        const std::string path_origin = std::join("/",std::vector<std::string>({new_file_dir_path,
                                                                                std::to_string(id_medecin),
                                                                                std::to_string(id_patient),
                                                                                filename}));
        //TODO test if file exist
        FILE* source = ::fopen(path_origin.c_str(), "r");
        if(source != nullptr)
        {
            std::cout<<"Save file "<<path_origin<<std::endl;
            //lock it
            if(flock(::fileno(source),LOCK_EX) == 0)
            {
                //get list of dest on network
                auto dests = Stats::get_nodes();
                //send them
                if(dests.size() > 0)
                {
                    for(auto& dest : dests)
                        res += cpForUpload(id_medecin,id_patient,filename,dest->host,dest->port,source);
                    //::remove(path_origin.c_str());
                }
                //unlock it
                ::flock(::fileno(source), LOCK_UN);
            }
            else
            {
                std::cerr<<"Unable to lock file "<<path_origin<<std::endl;
            }
            ::fclose(source);
        }
        //TODO rm path_origin
        return res;
    }

    std::string FileManager::makeNewFilePath(int id_medecin,int id_patient,const std::string& filename)
    {
        std::string res;
        if(id_patient >0 and filename != "")
            res = std::join("/",std::vector<std::string>({new_file_dir_path,
                                                         std::to_string(id_medecin),
                                                         std::to_string(id_patient),
                                                         filename}));
        else if(id_patient >0)
            res = std::join("/",std::vector<std::string>({new_file_dir_path,
                                                         std::to_string(id_medecin),
                                                         std::to_string(id_patient)}));
        else
            res = std::join("/",std::vector<std::string>({new_file_dir_path,
                                                         std::to_string(id_medecin)}));

        return res;
    }

    bool FileManager::cpForUpload(int id_medecin,int id_patient,std::string filename,std::string host, int port,FILE* source)
    {
        bool res = true;

        std::string path_dest = std::join("/",std::vector<std::string>({network_file_dir_path,
                                                                             host+":"+std::to_string(port),
                                                                             std::to_string(id_medecin),
                                                                             std::to_string(id_patient)}));
        if(std::createDir(path_dest) == 0)
            return false;
        path_dest +="/"+filename;

        FILE* dest = ::fopen(path_dest.c_str(), "wb");
        if(dest)
        {
            //cp source to dest
            ::fseek(source,0,SEEK_SET);

            char buf[BUFSIZ];
            size_t size;
            while ((size = ::fread(buf, 1, BUFSIZ, source)) > 0)
                ::fwrite(buf, 1, size, dest);

            ::fclose(dest);
        }
        else
            res = false;
        std::cout<<"copy file to "<<path_dest<<" ["<<res<<"]"<<std::endl;
        return res;
    }

}
