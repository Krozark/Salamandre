#include <Salamandre-daemon/FileManager.hpp>
#include <Salamandre-stats/stats.hpp>

#include <sstream>
#include <cstdlib>
#include <sys/file.h>

#include <utils/string.hpp>
#include <utils/sys.hpp>

constexpr int HEADER_SIZE = ntw::Serializer::Size<long int>::value;

namespace salamandre
{
    const std::string FileManager::new_file_dir_path = "datas/tosave";
    const std::string FileManager::network_file_dir_path = "datas/network";
    const std::string FileManager::backup_file_dir_path = "datas/backup";

    bool FileManager::prepareForUpload(int id_medecin)
    {
        int res = 0;
        const std::string path_medecin = utils::string::join("/",std::vector<std::string>({new_file_dir_path,
                                                                                std::to_string(id_medecin)}));
        //check if the medecin dir exist
        //if(std::createDir(path_medecin) == 2) //already exist
        {
            //check if the patient dir exist
            const std::list<std::string> patients = utils::sys::dir::list_dirs(path_medecin);
            for(const std::string& patient : patients)
                res += prepareForUpload(id_medecin,::atoi(patient.c_str()));
        }
        utils::sys::dir::rm_if_empty(path_medecin);
        return res;
    }

    bool FileManager::prepareForUpload(int id_medecin,int id_patient)
    {
        int res = 0;
        //patient path
        const std::string path_patient = utils::string::join("/",std::vector<std::string>({new_file_dir_path,
                                                                                std::to_string(id_medecin),
                                                                                std::to_string(id_patient)}));
        //if(std::createDir(path_patient) == 2) //already exist
        {
            //get list of files
            const std::list<std::string> files = utils::sys::dir::list_files(path_patient);
            for(const std::string& file : files)
                res += prepareForUpload(id_medecin,id_patient,file);
        }
        utils::sys::dir::rm_if_empty(path_patient);
        return res;
    }

    bool FileManager::prepareForUpload(int id_medecin,int id_patient,std::string filename)
    {
        int res = 0;
        const std::string path_origin = utils::string::join("/",std::vector<std::string>({new_file_dir_path,
                                                                                std::to_string(id_medecin),
                                                                                std::to_string(id_patient),
                                                                                filename}));
        //TODO test if file exist
        FILE* source = ::fopen(path_origin.c_str(), "rb");
        if(source != nullptr)
        {
            std::cout<<"Save file "<<path_origin<<std::endl;
            //lock it
            if(flock(::fileno(source),LOCK_EX) == 0)
            {
                //get list of dest on network
                auto dests = stats::Stats::get_nodes();
                //send them
                if(dests.size() > 0)
                {
                    for(auto& dest : dests)
                        res += cpForUpload(id_medecin,id_patient,filename,dest->host,dest->port,source);
                    //remove file
                    utils::sys::file::rm(path_origin);
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
        return res;
    }

    std::string FileManager::makeNewFilePath(int id_medecin,int id_patient,const std::string& filename,const std::string& folder)
    {
        std::string res;
        if(id_patient >0 and filename != "")
            res = utils::string::join("/",std::vector<std::string>({folder,
                                                         std::to_string(id_medecin),
                                                         std::to_string(id_patient),
                                                         filename}));
        else if(id_patient >0)
            res = utils::string::join("/",std::vector<std::string>({folder,
                                                         std::to_string(id_medecin),
                                                         std::to_string(id_patient)}));
        else
            res = utils::string::join("/",std::vector<std::string>({folder,
                                                         std::to_string(id_medecin)}));

        return res;
    }

    std::list<FileManager::FileInfo> FileManager::list(int id_medecin,int id_patient, const std::string& filename)
    {
        std::list<FileManager::FileInfo> res;
        if(id_medecin > 0 and id_patient >0 and filename != "")
        {
            list_append(id_medecin,id_patient,filename,res);
        }
        else if(id_medecin > 0 and id_patient >0)
        {
            list_append(id_medecin,id_patient,res);
        }
        else if(id_medecin > 0)
        {
            list_append(id_medecin,res);
        }
        return res;
    }

    void FileManager::init()
    {
        utils::sys::dir::create(new_file_dir_path);
    }

    void FileManager::list_append(int id_medecin,std::list<FileManager::FileInfo>& l)
    {
        const std::string path_medecin = utils::string::join("/",std::vector<std::string>({backup_file_dir_path,
                                                                                std::to_string(id_medecin)}));
        const std::list<std::string> patients = utils::sys::dir::list_dirs(path_medecin);
        for(const std::string& patient : patients)
            list_append(id_medecin,::atoi(patient.c_str()),l);
    }

    void FileManager::list_append(int id_medecin,int id_patient,std::list<FileManager::FileInfo>& l)
    {
        const std::string path_patient = utils::string::join("/",std::vector<std::string>({backup_file_dir_path,
                                                                                std::to_string(id_medecin),
                                                                                std::to_string(id_patient)}));

        const std::list<std::string> files = utils::sys::dir::list_files(path_patient);
        for(const std::string& file : files)
            list_append(id_medecin,id_patient,file,l);
    }

    void FileManager::list_append(int id_medecin,int id_patient, const std::string& filename,std::list<FileManager::FileInfo>& l)
    {
        const std::string path = utils::string::join("/",std::vector<std::string>({backup_file_dir_path,
                                                                                std::to_string(id_medecin),
                                                                                std::to_string(id_patient),
                                                                                filename}));
        FILE* f = ::fopen(path.c_str(),"rb");
        if(f != NULL)
        {
            //if(flock(::fileno(f),LOCK_EX) == 0)
            {
                char header[HEADER_SIZE];
                //read header
                if(fread(header,HEADER_SIZE,1,f) >= HEADER_SIZE)
                {
                    //unserialize header
                    long int version;
                    ntw::Serializer::convert(header,version);

                    /*ntw::Serializer serializer;
                    serializer.write(header,HEADER_SIZE);
                    //get version
                    serializer>>version;*/

                    FileInfo file = {
                        .version=version,
                        .id_medecin=id_medecin,
                        .id_patient=id_patient,
                        .filename=filename
                    };
                    l.push_back(std::move(file));
                }
                //::flock(::fileno(f), LOCK_UN);
            }
            ::fclose(f);
        }
    }

    ntw::Serializer& operator<<(ntw::Serializer& ser,const FileManager::FileInfo& self)
    {
        ser<<self.version<<self.id_medecin<<self.id_patient<<self.filename;
        return ser;
    }

    ntw::Serializer& operator>>(ntw::Serializer& ser,FileManager::FileInfo& self)
    {
        ser>>self.version>>self.id_medecin>>self.id_patient>>self.filename;
        return ser;
    }

    bool FileManager::cpForUpload(int id_medecin,int id_patient,std::string filename,std::string host, int port,FILE* source)
    {
        bool res = true;

        std::string path_dest = utils::string::join("/",std::vector<std::string>({network_file_dir_path,
                                                                             host+":"+std::to_string(port),
                                                                             std::to_string(id_medecin),
                                                                             std::to_string(id_patient)}));
        if(utils::sys::dir::create(path_dest) == 0)
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
