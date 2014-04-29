#include "functions.hpp"
///data base
#include <Socket/client/Client.hpp>
#include <Salamandre-daemon/GuiFunctions.hpp>

#include <iostream>


#define SERVER_PORT 1

#define PRINT_ERROR std::cout<<"Aucune action corespond à la demmande \""<<c<<"\""<<std::endl
#define BACK std::cout<<"Retour au menu précédant" <<std::endl;

void run(ntw::cli::Client& cli);

int main(int argc,char* argv[])
{
    if(argc < SERVER_PORT+1)
    {
        std::cout<<"Usage are: "<<argv[0]<<" <server-port>"<<std::endl;
        return 1;
    }


    ntw::Socket::init();
    ntw::cli::Client client;
    if(client.connect("127.0.0.1",atoi(argv[SERVER_PORT])) != NTW_ERROR_CONNEXION)
    {
        run(client);
    }

    ntw::Socket::close();

    return 0;
}

void run(ntw::cli::Client& client)
{
    int id_medecin, id_patient;
    std::cout<<"Id medecin:\n>";
    std::cin>>id_medecin;

    std::cout<<"Id patient:\n>";
    std::cin>>id_patient;

    std::vector<std::string> file_paths;
    std::vector<std::string> file_to_signal;

    char c = 0;
    while(c!= 'Q' and c != 'q')
    {
        std::cout<<
            "==========================\n"
            "=== Choisir une action ===\n"
            "==========================\n"
            "\t[1] Créer les fiches client\n"
            "\t[2] Déplacer des fiches vers le dossier de sauvgarde\n"
            "\t[3] Signaler la présence de nouveaux fichiers\n"
            "\t[4] Demmander une récupération de ficher\n"
            "\t[5] Verifier si une update est en cour\n"
            "\t[6] Voir la liste des fichier à sauver\n"
            "\t[Q/q] Quitter\n>";

        std::cin>>c;
        std::cout<<std::endl;

        switch(c)
        {
            case '1' ://créer fiches
            {
                while(c != 'q' and c != 'Q')
                {
                    std::cout<< 
                        "---------------------\n"
                        "--- Fiche à créer ---\n"
                        "---------------------\n"
                        "\t[1] test1\n"
                        "\t[2] test2\n"
                        "\t[3] test3\n"
                        "\t[4] test4\n"
                        "\t[5] test[1~4]\n"
                        "\t[6] Autre nom\n"
                        "\t[Q/q] Retour\n>";

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
                            std::cout<<"Nom du fichier à créer\n>";
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
                    test::moveForSave(id_medecin,id_patient,file,file_to_signal);
                file_paths.clear();
            {
            }break;
            case '3': //signaler nouveau fichiers
            {
                for(auto& file : file_to_signal)
                {
                    client.call<void>(salamandre::gui::func::newFile,id_medecin,id_patient,file);
                    int status = client.request_sock.getStatus();
                    switch(status)
                    {
                        case salamandre::gui::status::STOP :
                        {
                            std::cerr<<"[ERROR] The server is probably down."<<std::endl;
                            std::cout<<"[Recv] Stop"<<std::endl
                                <<"The programme will now stop"<<std::endl;
                            return;
                        }break;
                        default :
                        {
                            std::cout<<"[ERROR] Recv server code <"<<status<<"> whene sending file : "<<salamandre::gui::statusToString(status)<<std::endl;
                            /// server error???
                        }break;
                    }
                    client.request_sock.clear();
                }
                file_to_signal.clear();
            }break;
            case '4'://recupération
            {
            }break;
            case '5': ///update en cour
            {
            }break;
            case '6': //print file list
            {
                for(auto& file : file_paths)
                    std::cout<<"-\""<<ROOT_DIR<<id_medecin<<"/"<<id_patient<<"/"<<file<<"\""<<std::endl;
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
