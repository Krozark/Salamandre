#include "functions.hpp"
///data base
#include <Socket/client/Client.hpp>
#include <Socket/server/Server.hpp>
#include <Socket/FuncWrapper.hpp>

#include <Salamandre-daemon/GuiFunctions.hpp>
#include <utils/log.hpp>

#include <iostream>
#include <list>

#define SERVER_PORT 4321

#define PRINT_ERROR utils::log::error(std::to_string(c),"Aucune action corespond à la demmande");
#define BACK utils::log::info("Retour au menu précédant");

#define COLOR_TITLE utils::log::colors::light_blue
#define COLOR_ASK utils::log::colors::light_yellow
#define COLOR_RESET utils::log::colors::reset

void run(ntw::cli::Client& cli);
int check_status(ntw::cli::Client& cli);///< return 0 if ok, -1 if srv down, 1 on other errors (and print it)

int notification_dispatch(int id,ntw::SocketSerialized& request);///< dispatch function for notifier

int main(int argc,char* argv[])
{
    if(argc < 2)
    {
        utils::log::critical(1,"Usage are",argv[0],"<server-port>");
    }


    ntw::Socket::init();
    ntw::cli::Client client;
    if(client.connect("127.0.0.1",atoi(argv[1])) != ntw::Status::connexion)
    {
        //init notification listener
        ntw::srv::Server notification_srv(SERVER_PORT,"127.0.0.1",notification_dispatch,1,1);
        //notification_srv.on_new_client = void (*)(ntw::srv::Server& self,ntw::srv::Client& client);
        //notification_srv.on_delete_client = void (*)(ntw::srv::Server& self,ntw::srv::Client& client);
        notification_srv.start();

        //set the connect port on daémon side
        client.call<void>(salamandre::gui::func::setGuiNotificationPort,SERVER_PORT);
        if(check_status(client) < 0)
        {
            utils::log::critical(1,"Error on init notification server");
        }

        //start
        run(client);
        client.disconnect();

        //wait for end
        notification_srv.stop();
        notification_srv.wait();
    }

    ntw::Socket::close();

    return 0;
}


void run(ntw::cli::Client& client)
{
    int id_medecin, id_patient;
    std::cout<<COLOR_ASK<<"Id medecin:\n>"<<COLOR_RESET;
    std::cin>>id_medecin;

    std::cout<<COLOR_ASK<<"Id patient:\n>"<<COLOR_RESET;
    std::cin>>id_patient;

    std::vector<std::string> file_paths;
    std::vector<std::string> file_to_signal;

    std::string daemon_save_path = client.call<std::string>(salamandre::gui::func::getMySavePath);
    std::string daemon_backup_path = client.call<std::string>(salamandre::gui::func::getMyBackupPath);
    std::string daemon_bin_path = client.call<std::string>(salamandre::gui::func::getMyBinPath);

    char c = 0;
    while(c!= 'Q' and c != 'q')
    {
        std::cout<<COLOR_TITLE<<
            "==========================\n"
            "=== Choisir une action ===\n"
            "==========================\n"
            "\t[1] Créer les fiches client\n"
            "\t[2] Déplacer des fiches vers le dossier de sauvgarde\n"
            "\t[3] Signaler la présence de nouveaux fichiers\n"
            "\t[4] Demmander une récupération de ficher\n"
            "\t[5] Verifier si une update est en cour\n"
            "\t[6] Voir la liste des fichier à sauver\n"
            "\t[7] Voir le path du server\n"
            "\t[Q/q] Quitter\n>"
            <<COLOR_RESET;

        std::cin>>c;
        std::cout<<std::endl;

        switch(c)
        {
            case '1' ://créer fiches
            {
                while(c != 'q' and c != 'Q')
                {
                    std::cout<<COLOR_TITLE<<
                        "---------------------\n"
                        "--- Fiche à créer ---\n"
                        "---------------------\n"
                        "\t[1] test1\n"
                        "\t[2] test2\n"
                        "\t[3] test3\n"
                        "\t[4] test4\n"
                        "\t[5] test[1~4]\n"
                        "\t[6] Autre nom\n"
                        "\t[Q/q] Retour\n>"
                        <<COLOR_RESET;

                    std::cin>>c;
                    std::cout<<std::endl;

                    switch(c)
                    {
                        case '1'://test1
                        {
                            test::createFile(id_medecin,id_patient,"test1",file_paths);
                        }break;
                        case '2'://test2
                        {
                            test::createFile(id_medecin,id_patient,"test2",file_paths);
                        }break;
                        case '3'://test3
                        {
                            test::createFile(id_medecin,id_patient,"test3",file_paths);
                        }break;
                        case '4'://test4
                        {
                            test::createFile(id_medecin,id_patient,"test4",file_paths);
                        }break;
                        case '5'://test[1~4]
                        {
                            test::createFile(id_medecin,id_patient,"test1",file_paths);
                            test::createFile(id_medecin,id_patient,"test2",file_paths);
                            test::createFile(id_medecin,id_patient,"test3",file_paths);
                            test::createFile(id_medecin,id_patient,"test4",file_paths);
                        }break;
                        case '6'://autre
                        {
                            std::cout<<COLOR_ASK<<"Nom du fichier à créer\n>"<<COLOR_RESET;
                            std::string s;
                            std::cin>>s;
                            test::createFile(id_medecin,id_patient,s,file_paths);
                        }break;
                        case 'q'://quitter
                        case 'Q':
                        {
                            BACK;
                        }break;
                        default:
                        {
                            PRINT_ERROR;
                        }break;
                    }
                }
                c=0;
            }break;
            case '2': //déplacer vers save
                for(auto& file : file_paths)
                    test::moveForSave(id_medecin,id_patient,file,file_to_signal,daemon_save_path);
                file_paths.clear();
            {
            }break;
            case '3': //signaler nouveau fichiers
            {
                for(auto& file : file_to_signal)
                {
                    client.call<void>(salamandre::gui::func::newFile,id_medecin,id_patient,file);
                    if(check_status(client) < 0)
                    {
                        utils::log::critical(2,file,"ERROR on send new file info");
                    }
                    client.request_sock.clear();
                }
                file_to_signal.clear();
            }break;
            case '4'://recupération
            {
                while(c != 'q' and c != 'Q')
                {
                    std::cout<<COLOR_TITLE<< 
                        "----------------------------\n"
                        "--- Fichiers à récupérer ---\n"
                        "----------------------------\n"
                        "\t[1] test1\n"
                        "\t[2] test2\n"
                        "\t[3] test3\n"
                        "\t[4] test4\n"
                        "\t[5] test[1~4]\n"
                        "\t[6] Autre nom\n"
                        "\t[7] Tous ceux du patient\n"
                        "\t[8] Tous ceux du medecin\n"
                        "\t[Q/q] Retour\n>"
                        <<COLOR_RESET;

                    std::cin>>c;
                    std::cout<<std::endl;
                    std::list<std::string> files;

                    switch(c)
                    {
                        case '1'://test1
                        {
                            files.push_back("test1");
                        }break;
                        case '2'://test2
                        {
                            files.push_back("test2");
                        }break;
                        case '3'://test3
                        {
                            files.push_back("test3");
                        }break;
                        case '4'://test4
                        {
                            files.push_back("test4");
                        }break;
                        case '5'://test[1~4]
                        {
                            files.push_back("test1");
                            files.push_back("test2");
                            files.push_back("test3");
                            files.push_back("test4");
                        }break;
                        case '6'://autre
                        {
                            std::cout<<COLOR_ASK<<"Nom du fichier à créer\n>"<<COLOR_RESET;
                            std::string s;
                            std::cin>>s;
                            files.push_back(s);
                        }break;
                        case '7': //Tous ceux du patient
                        {
                            client.call<void>(salamandre::gui::func::sync,id_medecin,id_patient,"");
                            if(check_status(client) < 0)
                            {
                                utils::log::critical(3,id_patient,"ERROR on ask for an sync on all file of the patient");
                            }
                            client.request_sock.clear();
                        }break;
                        case '8':/// Tous ceux du medecin
                        {
                            client.call<void>(salamandre::gui::func::sync,id_medecin,-1,"");
                            if(check_status(client) < 0)
                            {
                                utils::log::critical(4,id_medecin,"ERROR on ask for an sync on all file of the medecin");
                            }
                            client.request_sock.clear();

                        }break;
                        case 'q'://quitter
                        case 'Q':
                        {
                            BACK;
                        }break;
                        default:
                        {
                            PRINT_ERROR;
                        }break;
                    }
                    //ask for sync on files
                    for(auto& file : files)
                    {
                        client.call<void>(salamandre::gui::func::sync,id_medecin,id_patient,file);
                        if(check_status(client) < 0)
                        {
                            utils::log::critical(5,file,"ERROR on ask for an sync on file");
                        }
                        client.request_sock.clear();
                    }
                }
                c=0;
            }break;
            case '5': ///update en cour
            {
                while(c != 'q' and c != 'Q')
                {
                    std::cout<<COLOR_TITLE<< 
                        "----------------------\n"
                        "--- Update en cour ---\n"
                        "----------------------\n"
                        "\t[1] test1\n"
                        "\t[2] test2\n"
                        "\t[3] test3\n"
                        "\t[4] test4\n"
                        "\t[5] Autre nom\n"
                        "\t[6] Tous ceux du patient\n"
                        "\t[7] Tous ceux du medecin\n"
                        "\t[8] N'importe lequel\n"
                        "\t[Q/q] Retour\n>"
                        <<COLOR_RESET;

                    std::cin>>c;
                    std::cout<<std::endl;
                    std::list<std::string> files;


                    bool up = false;

                    switch(c)
                    {
                        case '1'://test1
                        {
                            up = client.call<bool>(salamandre::gui::func::isInUpdate,id_medecin,id_patient,"test1");
                        }break;
                        case '2'://test2
                        {
                            up = client.call<bool>(salamandre::gui::func::isInUpdate,id_medecin,id_patient,"test2");
                        }break;
                        case '3'://test3
                        {
                            up = client.call<bool>(salamandre::gui::func::isInUpdate,id_medecin,id_patient,"test3");
                        }break;
                        case '4'://test4
                        {
                            up = client.call<bool>(salamandre::gui::func::isInUpdate,id_medecin,id_patient,"test4");
                        }break;
                        case '5'://autre
                        {
                            std::cout<<COLOR_ASK<<"Nom du fichier à créer\n>"<<COLOR_RESET;
                            std::string s;
                            std::cin>>s;
                            up = client.call<bool>(salamandre::gui::func::isInUpdate,id_medecin,id_patient,s);
                        }break;
                        case '6': //Tous ceux du patient
                        {
                            up = client.call<bool>(salamandre::gui::func::isInUpdate,id_medecin,id_patient,"");
                        }break;
                        case '7':/// Tous ceux du medecin
                        {
                            up = client.call<bool>(salamandre::gui::func::isInUpdate,id_medecin-1,"");
                        }break;
                        case '8':/// N'imp
                        {
                            up = client.call<bool>(salamandre::gui::func::isInUpdate,-1,-1,"");
                        }break;
                        case 'q'://quitter
                        case 'Q':
                        {
                            BACK;
                        }break;
                        default:
                        {
                            PRINT_ERROR;
                        }break;
                    }

                    if(c>='1' and c <='8')
                    {
                        if(check_status(client) < 0)
                        {
                            utils::log::critical(2,"ERROR on ask for is_update");
                        }
                        else
                            std::cout<<"Update: "<<(up?"Oui":"Non")<<std::endl;
                        client.request_sock.clear();
                    }
                } 
                c=0;
            }break;
            case '6': //print file list
            {
                for(auto& file : file_paths)
                    std::cout<<"-\""<<ROOT_DIR<<id_medecin<<"/"<<id_patient<<"/"<<file<<"\""<<std::endl;
            }break;
            case '7': //print daemon_path
            {
                std::cout<<"Daemon bin path : \""<<daemon_bin_path<<"\""<<std::endl;
                std::cout<<"Daemon save path : \""<<daemon_save_path<<"\""<<std::endl;
                std::cout<<"Daemon backup path : \""<<daemon_backup_path<<"\""<<std::endl;
            }break;
            case 'q': //quitter
            case 'Q':
            {
                std::cout<<"Exit"<<std::endl;
            }break;
            default:
            {
                PRINT_ERROR;
            }break;

        }
    }
}

int check_status(ntw::cli::Client& client)
{
    int status = client.request_sock.getStatus();
    switch(status)
    {
        case ntw::Status::stop :
        {
            utils::log::error("Stop","The server is probably down.","The programme will now stop");
            return -1;
        }break;
        case ntw::Status::ok :
        {
            return 0;
        }
        default :
        {
            std::cout<<"[ERROR] Recv server code <"<<status<<"> whene sending file : "<<salamandre::gui::statusToString(status)<<std::endl;
            /// server error???
            return 1;
        }break;
    }
}

int notification_dispatch(int id,ntw::SocketSerialized& request)
{
        int res= ntw::Status::wrong_id;
        utils::log::info("notification_dispatch","id:",id);
        switch(id)
        {
            case salamandre::gui::func::fileIsSend :
            { 
                res = ntw::FuncWrapper::srv::exec(salamandre::gui::funcFileIsSend,request);
            }break;
            case salamandre::gui::func::fileIsRecv :
            {
                res = ntw::FuncWrapper::srv::exec(salamandre::gui::funcFileIsRecv,request);
            }break;
            case salamandre::gui::func::endOfSync :
            {
                res = ntw::FuncWrapper::srv::exec(salamandre::gui::funcEndOfSync,request);
            }break;
            default:
            {
                utils::log::error("notification_dispatch","Function of id",id,"not found");
            }break;
        }
        return res;
}

///\todo TODO
namespace salamandre
{
namespace gui {

    void funcFileIsSend(ntw::SocketSerialized& sock,int id_medecin, int id_patient, std::string filename)
    {
        std::cout<<"File send:"<<id_medecin<<"/"<<id_patient<<"/"<<filename<<std::endl;
    }

    void funcFileIsRecv(ntw::SocketSerialized& sock,int id_medecin, int id_patient, std::string filename)
    {
        std::cout<<"File recv:"<<id_medecin<<"/"<<id_patient<<"/"<<filename<<std::endl;
    }

    void funcEndOfSync(ntw::SocketSerialized& sock,int id_medecin,int id_patient,std::string filename)
    {
        utils::log::info("gui::funcEndOfSync","medecin:",id_medecin,"patient:",id_patient,"filename:",filename);
    }
}
}
