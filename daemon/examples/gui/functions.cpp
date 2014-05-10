#include "functions.hpp"

#include <Socket/SocketSerialized.hpp>
#include <Socket/client/Client.hpp>

#include <Salamandre-daemon/GuiFunctions.hpp>

#include <utils/string.hpp>
#include <utils/sys.hpp>

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
        std::string path_origin = utils::string::join("/",std::vector<std::string>({ROOT_DIR,
                                                         std::to_string(id_medecin),
                                                         std::to_string(id_patient)}));
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

    void moveForSave(int id_medecin,int id_patient,const std::string& filename,std::vector<std::string>& file_to_signal)
    {
        std::string path_origin = utils::string::join("/",std::vector<std::string>({ROOT_DIR,
                                                         std::to_string(id_medecin),
                                                         std::to_string(id_patient),
                                                         filename}));

        std::string path_dest = utils::string::join("/",std::vector<std::string>({"datas/tosave",
                                                         std::to_string(id_medecin),
                                                         std::to_string(id_patient),
                                                         filename}));
        utils::sys::dir::create(path_dest);
        path_dest+="/"+filename;

        std::cout<<"Save file "<<path_origin<<" to "<<path_dest<<std::endl;

        if(::rename(path_origin.c_str(),path_dest.c_str()) != 0)
            std::cerr<<"[Error] on move "<<path_origin<<" to "<<path_dest<<std::endl;
        else
            file_to_signal.push_back(filename);
    }
}
