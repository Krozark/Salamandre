#include <Salamandre-daemon/FileManager.hpp>
#include <Salamandre-daemon/ServerFunctions.hpp>
#include <Salamandre-daemon/GuiFunctions.hpp>
#include <Salamandre-daemon/Daemon.hpp>

#include <Salamandre-stats/stats.hpp>

#include <sstream>
#include <cstdlib>
#include <sys/file.h>

#include <utils/string.hpp>
#include <utils/sys.hpp>
#include <utils/log.hpp>

#include <Socket/Serializer.hpp>
#include <Socket/client/Client.hpp>

constexpr int HEADER_SIZE = ntw::Serializer::Size<uint64_t>::value;

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
        const std::string path_origin = utils::string::join("/",new_file_dir_path,id_medecin,id_patient,filename);
        //TODO test if file exist
        FILE* source = ::fopen(path_origin.c_str(), "rb");
        if(source != nullptr)
        {
            utils::log::info("FileManager::prepareForUpload","Save file ",path_origin);
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
                utils::log::error("FileManager::prepareForUpload","Unable to lock file.",path_origin);
            }
            ::fclose(source);
        }
        return res;
    }

    std::string FileManager::makeNewFilePath(int id_medecin,int id_patient,const std::string& filename,const std::string& folder)
    {
        std::string res;
        if(id_patient >0 and filename != "")
            res = utils::string::join("/",folder,id_medecin,id_patient,filename);
        else if(id_patient >0)
            res = utils::string::join("/",folder,id_medecin,id_patient);
        else
            res = utils::string::join("/",folder,id_medecin);

        return res;
    }

    std::list<FileInfo> FileManager::list(int id_medecin,int id_patient, const std::string& filename)
    {
        std::list<FileInfo> res;
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

    void FileManager::list_append(int id_medecin,std::list<FileInfo>& l)
    {
        const std::string path_medecin = utils::string::join("/",backup_file_dir_path,id_medecin);
        const std::list<std::string> patients = utils::sys::dir::list_dirs(path_medecin);

        for(const std::string& patient : patients)
            list_append(id_medecin,::atoi(patient.c_str()),l);
    }

    void FileManager::list_append(int id_medecin,int id_patient,std::list<FileInfo>& l)
    {
        const std::string path_patient = utils::string::join("/",backup_file_dir_path,id_medecin,id_patient);

        const std::list<std::string> files = utils::sys::dir::list_files(path_patient);
        for(const std::string& file : files)
            list_append(id_medecin,id_patient,file,l);
    }

    void FileManager::list_append(int id_medecin,int id_patient, const std::string& filename,std::list<FileInfo>& l)
    {
        const std::string path = utils::string::join("/",backup_file_dir_path,id_medecin,id_patient,filename);
        FILE* f = ::fopen(path.c_str(),"rb");
        if(f != nullptr)
        {
            if(flock(::fileno(f),LOCK_EX) == 0)
            {
                char header[HEADER_SIZE];
                //read header
                int read = ::fread(header,1,HEADER_SIZE,f);

                if(read == HEADER_SIZE)
                {
                    //unserialize header
                    uint64_t version;
                    ntw::Serializer::convert(header,version);

                    //ntw::Serializer serializer;
                    //serializer.write(header,HEADER_SIZE);
                    ////get version
                    //serializer>>version;

                    FileInfo file = {
                        .version=version,
                        .id_medecin=id_medecin,
                        .id_patient=id_patient,
                        .filename=filename
                    };
                    l.push_back(std::move(file));
                    
                }
                else
                    utils::log::error("FileInfo::list_append","fread");
                ::flock(::fileno(f), LOCK_UN);
            }
            ::fclose(f);
        }
    }


    bool FileManager::cpForUpload(int id_medecin,int id_patient,std::string filename,std::string host, int port,FILE* source)
    {
        bool res = true;

        std::string path_dest = utils::string::join("/",network_file_dir_path,host+":"+std::to_string(port),id_medecin,id_patient);
        if(utils::sys::dir::create(path_dest) == 0)
            return false;
        path_dest +="/"+filename;

        FILE* dest = ::fopen(path_dest.c_str(), "wb");
        if(dest != nullptr)
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
        utils::log::info("FileManager::cpForUpload","copy file to ",path_dest,". ok?",res);
        return res;
    }


    //////////// NON STATIC //////////////
    FileManager::FileManager(int t) : timeout(t)
    {
    }

    void FileManager::start()
    {
        utils::sys::dir::create(new_file_dir_path);
        utils::sys::dir::create(backup_file_dir_path);

        run = true;
        thread = std::thread(&FileManager::_start_thread,this);
    }

    void FileManager::stop()
    {
        run = false;
    }

    void FileManager::wait()
    {
        thread.join();
    }

    void FileManager::_start_thread()
    {
        const std::chrono::seconds duration(timeout);
        const std::chrono::milliseconds step(5000);

        std::chrono::milliseconds elapsed_time(0);

        build_list_to_prepare();
        build_list_to_send();

        while(run)
        {
            if (elapsed_time > duration)
            {
                build_list_to_prepare();
                build_list_to_send();
                const auto end = files_to_send.end();
                for(auto current = files_to_send.begin(); current!= end;)
                {
                    if(send_file(*current))
                    {
                        current = files_to_send.erase(current);
                    }
                    else
                        ++current;
                }
                elapsed_time -= duration;
            }
            std::this_thread::sleep_for(step);
            elapsed_time += step;
        }
    }

    void FileManager::build_list_to_send()
    {
        files_to_send.clear();

        const std::list<std::string> dest_l = utils::sys::dir::list_dirs(network_file_dir_path);
        for(const std::string& dest : dest_l)
        {
            const std::string dest_path = utils::string::join("/",network_file_dir_path,dest);
            const std::list<std::string> medecin_l = utils::sys::dir::list_dirs(dest_path);


            std::vector<std::string> ip_port = utils::string::split(dest,":");

            if(ip_port.size() != 2)
                continue;


            const int port = ::atoi(ip_port[1].c_str());

            for(const std::string& medecin : medecin_l)
            {
                const std::string medecin_path = utils::string::join("/",dest_path,medecin);
                const std::list<std::string> patient_l = utils::sys::dir::list_dirs(medecin_path);

                int id_medecin = ::atoi(medecin.c_str());

                for(const std::string& patient : patient_l)
                {
                    const std::string patient_path = utils::string::join("/",medecin_path,patient);
                    const std::list<std::string> file_l = utils::sys::dir::list_files(patient_path);

                    int id_patient = ::atoi(patient.c_str());

                    for(const std::string& file : file_l)
                    {
                        const std::string file_path = utils::string::join("/",patient_path,file);
                        
                        FileInfoFromPath tmp;
                        tmp.id_medecin = id_medecin;
                        tmp.id_patient = id_patient;
                        tmp.filename = file;
                        tmp.path = file_path;
                        tmp.port = port;
                        tmp.ip = ip_port[0];

                        files_to_send.insert(tmp);

                        std::cout<<"Add path "<<file_path<<" to send"<<std::endl;
                    }
                }
            }
        }
    }

    void FileManager::build_list_to_prepare()
    {
        const std::list<std::string> medecin_l = utils::sys::dir::list_dirs(new_file_dir_path);
        for(const std::string& medecin : medecin_l)
        {
            const std::string medecin_path = utils::string::join("/",new_file_dir_path,medecin);
            const std::list<std::string> patient_l = utils::sys::dir::list_dirs(medecin_path);

            int id_medecin = ::atoi(medecin.c_str());

            for(const std::string& patient : patient_l)
            {
                const std::string patient_path = utils::string::join("/",medecin_path,patient);
                const std::list<std::string> file_l = utils::sys::dir::list_files(patient_path);

                int id_patient = ::atoi(patient.c_str());

                for(const std::string& file : file_l)
                {
                    prepareForUpload(id_medecin,id_patient,file);
                }
                utils::sys::dir::rm_if_empty(patient_path);
            }
            utils::sys::dir::rm_if_empty(medecin_path);
        }

    }

    bool FileManager::send_file(const salamandre::FileInfoFromPath& info)
    {
        bool res = false;
        ntw::cli::Client* client = new ntw::cli::Client;

        if(client->connect(info.ip,info.port) == ntw::Status::ok)
        {
            FILE* f = ::fopen(info.path.c_str(),"rb");
            if(f != nullptr)
            {
                if(::flock(::fileno(f),LOCK_EX|LOCK_NB) == 0)
                {
                    utils::log::info("FileManager::send_file","to ip:",info.ip,"port:",info.port,"file:",info.path);

                    res = true;
                    std::thread thread([client,f,info]()->void {
                        ::fseek(f,0,SEEK_SET);
                        char buf[BUFSIZ];
                        size_t size; //file size
                        client->request_sock<<salamandre::srv::saveThisFile
                            <<info.id_medecin
                            <<info.id_patient
                            <<info.filename;
                        //add file datas
                        while ((size = ::fread(buf, 1, BUFSIZ,f)) > 0)
                            client->request_sock.write(buf,size);

                        client->request_sock.sendCl();
                        if(client->request_sock.receive() > 0)
                        {
                            utils::sys::file::rm(info.path);
                            std::string dest_path = utils::string::join("/",network_file_dir_path,info.ip+":"+std::to_string(info.port));
                            /*str::string medecin_path = utils::string::join("/",dest_path,id_medecin);
                            std::string patient_path = utils::string::join("/",medecin_path,id_patient);
                            utils::sys::dir::rm_if_empty(medecin_path);
                            utils::sys::dir::rm_if_empty(patient_path);*/
                            utils::sys::dir::rm_if_empty(dest_path,true);
                        }
                        delete client;
                        ::flock(::fileno(f),LOCK_UN);
                        ::fclose(f);

                        daemon->notify(salamandre::gui::func::fileIsSend,info.id_medecin,info.id_patient,info.filename);
                    });
                    thread.detach();
                }
                else
                    ::fclose(f);

            }
        }

        if(res == false)
        {
            delete client;
        }
        return res;
    }

}
