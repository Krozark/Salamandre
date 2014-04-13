#include "functions.hpp"

#include <Socket/SocketSerialized.hpp>
#include <Socket/client/Client.hpp>

#include <Salamandre-daemon/GuiFunctions.hpp>
#include <Salamandre-daemon/FileManager.hpp>
#include <Salamandre-daemon/std.hpp>

#include <sstream>
#include <ctime>
#include <random>

namespace test
{
    void init()
    {
        ::srand(time(NULL));
    }

    int random(int min,int max)
    {
        return rand()%(max-min+1) + min;
    }

    void createFile(int id_medecin,int id_patient,const std::string& filename)
    {
        //compute the path
        std::string path_origin = salamandre::FileManager::makeNewFilePath(id_medecin,id_patient);
        //create new dir if not exist
        std::createDir(path_origin);

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
    }
    
    void sendNewFile(ntw::cli::Client& client)
    {
        int id_medecin = random(0,1000);
        int id_patient = random(0,1000);

        auto init_files = [=](){
            createFile(id_medecin,id_patient,"test1");
            createFile(id_medecin,id_patient,"test2");
            createFile(id_medecin,id_patient,"test3");
            createFile(id_medecin,id_patient,"test4");
        };

        //all params
        /*init_files();
        client.call<void>(salamandre::gui::func::newFile,id_medecin,id_patient,"test1");
        client.call<void>(salamandre::gui::func::newFile,id_medecin,id_patient,"test2");
        client.call<void>(salamandre::gui::func::newFile,id_medecin,id_patient,"test3");
        client.call<void>(salamandre::gui::func::newFile,id_medecin,id_patient,"test4");*/
        //no filename
        init_files();
        client.call<void>(salamandre::gui::func::newFile,id_medecin,id_patient,"");
        //no patient
        /*init_files();
        client.call<void>(salamandre::gui::func::newFile,id_medecin,-1,"");*/

        short int status = client.request_sock.getStatus();
        switch(status)
        {
            case salamandre::gui::status::STOP :
                {
                    std::cerr<<" : The server is probably down."<<std::endl;
                    std::cout<<"[Recv] Stop"<<std::endl
                        <<"The programme will now stop"<<std::endl;
                    client.request_sock.clear();
                }break;
            default :
                {
                    std::cout<<"[Recv] Server error code:"<<status<<":"<<salamandre::gui::statusToString((salamandre::gui::status)status)<<std::endl;
                    client.request_sock.clear();
                    /// server error???
                }break;
        }
    }

}
