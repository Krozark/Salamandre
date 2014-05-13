#include "functions.hpp"

#include <Socket/SocketSerialized.hpp>
#include <Socket/client/Client.hpp>

#include <Salamandre-daemon/GuiFunctions.hpp>

#include <utils/string.hpp>
#include <utils/sys.hpp>
#include <utils/log.hpp>

#include <sstream>
#include <ctime>
#include <random>



namespace test
{
    int random(int min,int max)
    {
        return rand()%(max-min+1) + min;
    }

    void createFile(int id_medecin,int id_patient,const std::string& filename,std::vector<std::string>& file_paths)
    {
        //compute the path
        std::string path_origin = utils::string::join("/",ROOT_DIR,id_medecin,id_patient);
        //create new dir if not exist
        utils::sys::dir::create(path_origin);

        path_origin+="/"+filename;
        std::cout<<"Creat file "<<path_origin<<std::endl;
        //open new file
        FILE* source = ::fopen(path_origin.c_str(), "wb"); 
        //add random datas
        char buf[BUFSIZ];
        for(unsigned int i=0;i<BUFSIZ;++i)
            buf[i] = (char)random(33,126);
        for(int i=0;i<10;++i)
            ::fwrite(buf, 1, BUFSIZ, source);
        //end
        fclose(source);
        file_paths.push_back(filename);
    }

    void moveForSave(int id_medecin,int id_patient,const std::string& filename,std::vector<std::string>& file_to_signal,const std::string& to)
    {
        std::string path_origin = utils::string::join("/",ROOT_DIR,id_medecin,id_patient,filename);

        std::string path_dest = utils::string::join("/",to,id_medecin,id_patient);
        utils::sys::dir::create(path_dest);
        path_dest+="/"+filename;

        std::cout<<"Save file "<<path_origin<<" to "<<path_dest<<std::endl;

        if(::rename(path_origin.c_str(),path_dest.c_str()) != 0)
            utils::log::error("Move",path_origin,"to",path_dest);
        else
        {
            file_to_signal.push_back(filename);
            //rm datas/tosave
            std::string path = utils::string::join("/",to,id_medecin);
            utils::sys::dir::rm_if_empty(path,true);

            //rm gui/save
            path = utils::string::join("/",ROOT_DIR,id_medecin);
            utils::sys::dir::rm_if_empty(path,true);
        }
    }
}
